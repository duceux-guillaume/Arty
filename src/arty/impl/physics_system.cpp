#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Blackboard>& board) {
  auto phyPtr = board->getProperties<Physics>("physics");
  auto posPtr = board->getProperties<Transform>("transform");
  if (!phyPtr) {
    return error("no physics");
  }
  auto phyIt = phyPtr->begin();
  auto phyEnd = phyPtr->end();
  if (!posPtr) {
    // no transforms yet
    for (; phyIt != phyEnd; ++phyIt) {
      board->set(phyIt->entity, OUTPUT_PROP, Transform());
    }
    return ok();
  }
  auto posIt = posPtr->begin();
  for (; phyIt != phyEnd; ++phyIt, ++posIt) {
    // I've got physics but not transform, let's add one
    while (posIt->entity > phyIt->entity) {
      board->set(phyIt->entity, OUTPUT_PROP, Transform());
      ++phyIt;
    }
    // I've got transform but no physics, it's ok
    if (posIt->entity < phyIt->entity) {
      continue;
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
  tf->translation += phy->velocity.translation * 0.016f;
  phy->velocity.translation += phy->acceleration.translation * 0.016f;
  phy->velocity.translation *= world.air_friction;
  phy->velocity.translation *= world.ground_friction;
  // handle gravity
  Vec3f gravity;
  if (tf->translation.z() > world.min_ground_level) {
    gravity = Vec3f(0.f, 0.f, -1.f) * world.gravity_strengh;
  }
  // handle forces
  Vec3f force = gravity;
  assert(phy->forces_pos.size() == phy->forces_dir.size());
  for (std::size_t i = 0; i < phy->forces_pos.size(); ++i) {
    // compute comp trans
    /*Vec3f pos = phy->forces_pos[i];*/
    force += phy->forces_dir[i];
  }
  auto tra = force * 1.f / phy->mass;
  phy->acceleration.translation = tra;
  phy->forces_dir.clear();
  phy->forces_pos.clear();
}

}  // namespace arty
