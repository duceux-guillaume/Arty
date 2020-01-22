#include <arty/impl/collision_solver_system.hpp>

namespace arty {

Result CollisionSolverSystem::process(Ptr<Memory> const& mem) {
  auto work = [&](Entity const& e, std::vector<Collision> const& b) -> Result {
    for (auto const& col : b) {
      auto tf = mem->read<Tf3f>(e, PhysicsSystem::INOUT_1);
      auto phy = mem->read<Physics>(e, PhysicsSystem::INOUT_2);
      _solver.update(&phy, &tf, col, e == col.e2);
      mem->write(e, PhysicsSystem::INOUT_1, tf);
    }
    return ok();
  };
  return mem->process<std::vector<Collision>>(INPUT, work);
}

Result CollisionSolverSystem::init(const Ptr<Memory>& /*board*/) {
  return ok();
}

void CollisionSolverSystem::release() {}

void CollisionSolver::update(Physics* phy, Tf3f* tf, const Collision& c,
                             int entity) {
  if (!phy->dynamic) {
    return;
  }
  // phy->forces_pos.push_back((c.shape.pts()[1] + c.shape.pts()[0]) * 0.5f);
  Vec3f dir = c.shape.pts()[0] - c.shape.pts()[1];
  if (entity == 1) {
    dir = c.shape.pts()[1] - c.shape.pts()[0];
  }
  tf->translation() += dir * 0.5f;
  // phy->forces_dir.push_back(dir);
}

}  // namespace arty
