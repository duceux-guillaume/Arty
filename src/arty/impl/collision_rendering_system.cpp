#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision.hpp>
#include <arty/impl/collision_rendering_system.hpp>

namespace arty {
Result CollisionRenderingSystem::process(const Ptr<Blackboard>& board) {
  // Import meshes
  auto ptr = board->getProperties<Shape3f>(IMPORT_PROP);
  if (ptr && ptr->size()) {
    for (auto const& prop : *ptr) {
      _renderer->import(prop);
      board->set(prop.entity, DRAW_PROP, prop.value);
    }
    check_result(board->clearProperties(IMPORT_PROP));
  }

  auto camPtr = board->getProperties<Camera>("camera");
  if (!camPtr || camPtr->empty()) {
    return error("no camera provided");
  }
  auto cam = camPtr->at(0).value;
  auto shapePtr = board->getProperties<Shape3f>(DRAW_PROP);
  if (!shapePtr || shapePtr->empty()) {
    return error("no shape properties");
  }
  auto posPtr = board->getProperties<Transform>("transform");
  if (!posPtr || posPtr->empty()) {
    return error("no transform properties");
  }

  auto shapeIt = shapePtr->begin();
  auto shapeEnd = shapePtr->end();
  auto posIt = posPtr->begin();

  for (; shapeIt != shapeEnd; ++shapeIt, ++posIt) {
    if (shapeIt->entity != posIt->entity) {
      return error("vbo doesn't match entity");
    }
    _renderer->draw(*shapeIt, posIt->value.toMat(), cam.view, cam.projection);
  }
  return ok();
}

Result CollisionRenderingSystem::init(const Ptr<Blackboard>& /*board*/) {
  check_result(_renderer->init());
  return ok();
}

void CollisionRenderingSystem::release() {}
}  // namespace arty
