#include <arty/impl/physics.hpp>

namespace arty {

void PhysicsSolver::update(WorldPhysics const& world, Physics* phy) {
  assert(phy);
  if (phy->isStatic()) {
    return;
  }
  float deltaTime = 0.016f;
  phy->move(phy->velocity().translation() * deltaTime);
  phy->impulse(phy->acceleration().translation() * deltaTime);
  // handle gravity
  Vec3f gravity = Vec3f(0.f, 0.f, -1.f) * world.gravity_strengh;
  phy->apply(gravity);
}

}  // namespace arty
