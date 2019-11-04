#include <arty/core/mesh.hpp>
#include <arty/impl/collision_rendering_system.hpp>
#include <arty/impl/collision_system.hpp>

namespace arty {
Result CollisionSystem::process(const Ptr<Blackboard> &board) {
  auto meshPtr = board->getProperties<Mesh>("mesh");
  auto posPtr = board->getProperties<Transform>("transform");
  if (!meshPtr || !posPtr) {
    return error("missing properties");
  }
  auto meshIt = meshPtr->begin();
  auto meshIt2 = meshPtr->begin();
  auto meshEnd = meshPtr->end();
  auto meshEnd2 = meshPtr->end();
  auto posIt = posPtr->begin();
  auto posIt2 = posPtr->begin();
  for (; meshIt != meshEnd; ++meshIt, ++posIt) {
    if (posIt->entity != meshIt->entity) {
      return error("transform doesn't match entity");
    }
    for (; meshIt2 != meshEnd2; ++meshIt2, ++posIt2) {
      if (posIt2->entity != meshIt2->entity) {
        return error("transform doesn't match entity");
      }
      if (posIt->entity == posIt2->entity) {
        continue;
      }
      std::cout << "detection between: " << meshIt->entity.name << " "
                << meshIt2->entity.name << std::endl;
      Collision col = _collision.detect(meshIt->value, posIt->value,
                                        meshIt2->value, posIt2->value);
      if (col.exist) {
        std::cout << "Found collision" << std::endl;
        board->set<Shape3f>(posIt->entity, CollisionRenderingSystem::DRAW_PROP,
                            col.shape);
      }
    }
  }
  return ok();
}

Result CollisionSystem::init(const Ptr<Blackboard> & /*board*/) { return ok(); }

void CollisionSystem::release() {}

}  // namespace arty
