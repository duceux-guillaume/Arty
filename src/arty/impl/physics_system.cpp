#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Memory>& mem) {
  auto work = [this](Entity const& e, Transform& t, Physics& p) -> Result {
    std::cout << "updated " << e.name() << " " << t.translation.z()
              << std::endl;
    _solver.update(&t, &p, _world);
    std::cout << "updated " << e.name() << " " << t.translation.z()
              << std::endl;
    return ok();
  };
  mem->update<Transform, Physics>("transform", "physics", work);
  return ok();
}

void PhysicsSolver::update(Transform* tf, Physics* phy,
                           const WorldPhysics& world) {
  assert(tf);
  assert(phy);
  if (phy->dynamic) {
    float deltaTime = 0.016f;
    tf->translation += phy->velocity.translation * deltaTime;
    phy->velocity.translation += phy->acceleration.translation * deltaTime;
    // handle gravity
    Vec3f gravity = Vec3f(0.f, 0.f, -1.f) * world.gravity_strengh;
    phy->acceleration = gravity;
  }
  /*
    // handle forces
    Vec3f force = gravity;
    assert(phy->forces_pos.size() == phy->forces_dir.size());
    for (std::size_t i = 0; i < phy->forces_pos.size(); ++i) {
      // compute comp trans
  force += phy->forces_dir[i];
}
auto tra = force * 1.f / phy -> mass;
phy->acceleration.translation = tra;
phy->forces_dir.clear();
phy->forces_pos.clear();
*/
}

}  // namespace arty
