#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/collision.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {

using CollisionArray = std::vector<Collision>;

class CollisionDetectionSystem : public System {
 private:
  CollisionDetection _collision;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
};
/*
class CollisionSolverSystem : public System {
 private:
  CollisionSolver _solver;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
};
*/
class CollisionRenderingSystem : public System {
 public:
  CollisionRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
  Result init(const Ptr<Memory>& board) override;
};

}  // namespace arty

#endif  // COLLISION_SYSTEM_HPP
