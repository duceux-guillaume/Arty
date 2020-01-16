#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {
Result HitBoxRenderingSystem::process(const Ptr<Blackboard>& board) {
  auto camPtr = board->getProperties<Camera>("camera");
  if (!camPtr || camPtr->empty()) {
    return error("no camera provided");
  }
  auto cam = camPtr->at(0).value;

  auto posPtr = board->getProperties<Transform>("transform");
  auto shapePtr = board->getProperties<Box>(DRAW_PROP);
  if (!shapePtr || !posPtr) {
    return error("no shape properties");
  }
  if (shapePtr->empty()) {
    return error("no shape to draw");
  }

  auto shapeIt = shapePtr->begin();
  auto shapeEnd = shapePtr->end();
  auto posIt = posPtr->begin();

  for (; shapeIt != shapeEnd; ++shapeIt, ++posIt) {
    if (posIt->entity != shapeIt->entity) {
      return error("transform doesn't match entity");
    }
    _renderer->draw(shapeIt->entity, shapeIt->value, posIt->value.toMat(),
                    cam.view, cam.projection);
  }
  return ok();
}

Result HitBoxRenderingSystem::init(const Ptr<Blackboard>& /*board*/) {
  check_result(_renderer->init());
  return ok();
}

void HitBoxRenderingSystem::release() {}
}  // namespace arty
