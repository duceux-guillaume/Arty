#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Blackboard>& board) {
  auto phyPtr = board->getProperties<Physics>("physics");
  auto posPtr = board->getProperties<Transform>("transform");
  if (!phyPtr || !posPtr) {
    return error("missing properties");
  }
  auto phyIt = phyPtr->begin();
  auto phyEnd = phyPtr->end();
  auto posIt = posPtr->begin();
  for (; phyIt != phyEnd; ++phyIt, ++posIt) {
    if (posIt->entity != phyIt->entity) {
      return error("transform doesn't match entity");
    }
    _solver.update(&posIt->value, &phyIt->value, _world);
  }
  return ok();
}

Result PhysicsSystem::init(const Ptr<Blackboard>& /*board*/) { return ok(); }

void PhysicsSystem::release() {}

void PhysicsSolver::update(Transform* tf, Physics* phy,
                           const WorldPhysics& world) {
  assert(tf);
  assert(phy);
  tf->translation += phy->velocity.translation;
  phy->velocity.translation += phy->acceleration.translation;
  // handle gravity
  if (tf->translation.z() > world.min_ground_level) {
    auto tra = Vec3f(0.f, 0.f, -1.f) * world.gravity_strengh * 1.f / phy->mass;
    phy->acceleration.translation = tra;
  } else {
    tf->translation.z() = 0.f;
  }
}

}  // namespace arty
