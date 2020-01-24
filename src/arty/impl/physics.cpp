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

  Vec3f gravity = Vec3f(0.f, 0.f, -1.f) * world.gravity_strengh;
  phy->apply(gravity);
  Physics::vector_type total;
  for (auto f : phy->forces()) {
    total += f;
  }
  phy->acceleration() = total * 1.f / phy->mass();
  phy->flush();
}

}  // namespace arty
