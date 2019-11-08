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
  auto meshEnd = meshPtr->end();
  auto posIt = posPtr->begin();
  for (; meshIt != meshEnd; ++meshIt, ++posIt) {
    if (posIt->entity != meshIt->entity) {
      return error("transform doesn't match entity");
    }
    auto aabb =
        Shape3f::box(_collision.computeAxisAlignedBoundingBox(meshIt->value));
    board->set(meshIt->entity, "aabb", aabb);
    auto meshIt2 = meshIt;
    auto posIt2 = posIt;
    for (; meshIt2 != meshEnd; ++meshIt2, ++posIt2) {
      if (posIt2->entity != meshIt2->entity) {
        return error("transform doesn't match entity");
      }
      if (posIt->entity == posIt2->entity) {
        continue;
      }
      /*
      Collision col = _collision.detect(meshIt->value, posIt->value,
                                        meshIt2->value, posIt2->value);
      if (col.exist) {
        std::cout << "Found collision" << std::endl;
        col.e1 = meshIt->entity;
        col.e2 = meshIt2->entity;
        board->set(meshIt->entity, "collision", col);
      } else {
        board->remove(meshIt->entity, "collision");
      }
      */
    }
  }
  return ok();
}

Result CollisionSystem::init(const Ptr<Blackboard> & /*board*/) { return ok(); }

void CollisionSystem::release() {}

}  // namespace arty
