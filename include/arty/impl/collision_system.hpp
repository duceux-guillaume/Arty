#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/collision.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {

class CollisionDetectionSystem : public System {
 public:
  static constexpr const char* INPUT_1 = "transform";
  static constexpr const char* INPUT_2 = "hitbox";
  static constexpr const char* OUTPUT = "collision";

 private:
  CollisionDetection _collision;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
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

class CollisionRenderingSystem : public System {
 public:
  static constexpr const char* INPUT = "collision";

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
