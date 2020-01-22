#ifndef COLLISION_SOLVER_SYSTEM_HPP
#define COLLISION_SOLVER_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/collision.hpp>
#include <arty/impl/physics_system.hpp>

namespace arty {

class CollisionSolver {
 public:
  void update(Physics* phy, Tf3f* tf, Collision const& c, int entity);
};

class CollisionSolverSystem : public System {
 private:
  static constexpr const char* INPUT = "collision";

  CollisionSolver _solver;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
  Result init(const Ptr<Memory>& board) override;
  void release() override;
};

}  // namespace arty

#endif  // COLLISION_SOLVER_SYSTEM_HPP
