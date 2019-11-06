#include <arty/impl/collision_solver_system.hpp>

namespace arty {

Result CollisionSolverSystem::process(const Ptr<Blackboard> &board) {
  auto colPtr = board->getProperties<Collision>("collision");
  if (!colPtr) {
    return ok();
  }

  for (auto const &col : *colPtr) {
    {
      Physics *phy = board->getEntityProperty<Physics>(col.value.e1, "physics");
      Transform *tf =
          board->getEntityProperty<Transform>(col.value.e1, "transform");
      if (!phy || !tf) {
        error("collision with nothing");
      }
      _solver.update(phy, tf, col.value, 0);
    }
    {
      Physics *phy = board->getEntityProperty<Physics>(col.value.e2, "physics");
      Transform *tf =
          board->getEntityProperty<Transform>(col.value.e2, "transform");
      if (!phy || !tf) {
        error("collision with nothing");
      }
      _solver.update(phy, tf, col.value, 1);
    }
  }
  return ok();
}

Result CollisionSolverSystem::init(const Ptr<Blackboard> & /*board*/) {
  return ok();
}

void CollisionSolverSystem::release() {}

void CollisionSolver::update(Physics *phy, Transform *tf, const Collision &c,
                             int entity) {
  phy->forces_pos.push_back((c.shape.pts()[1] + c.shape.pts()[0]) * 0.5f);
  Vec3f dir = c.shape.pts()[0] - c.shape.pts()[1];
  if (entity == 1) {
    dir = c.shape.pts()[1] - c.shape.pts()[0];
  }
  tf->translation += dir * 0.5f;
  phy->forces_dir.push_back(dir);
}

}  // namespace arty
