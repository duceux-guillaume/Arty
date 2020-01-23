#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Memory>& mem) {
  auto work = [mem, this](Entity const& e, Physics const& p) -> Result {
    auto newPhy = p;
    Tf3f newTf;
    _solver.update(&newTf, &newPhy, _world);
    mem->write(e, OUTPUT, newTf);
    mem->write(e, INPUT, newPhy);
    return ok();
  };
  mem->process<Physics>(INPUT, work);
  return ok();
}

void PhysicsSolver::update(Tf3f* tf, Physics* phy, const WorldPhysics& world) {
  assert(tf);
  assert(phy);
  if (phy->dynamic) {
    float deltaTime = 0.016f;
    phy->position.translation() += phy->velocity.translation() * deltaTime;
    phy->velocity.translation() += phy->acceleration.translation() * deltaTime;
    // handle gravity
    Vec3f gravity = Vec3f(0.f, 0.f, -1.f) * world.gravity_strengh;
    phy->acceleration.translation() = gravity;
  }
  *tf = phy->position;
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
