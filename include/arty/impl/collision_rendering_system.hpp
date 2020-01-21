#ifndef COLLISION_RENDERING_SYSTEM_HPP
#define COLLISION_RENDERING_SYSTEM_HPP

#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {

class CollisionRenderingSystem : public System {
 public:
  static constexpr const char *INPUT = "collision";

 public:
  CollisionRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Memory> &board) override;
  Result init(const Ptr<Memory> &board) override;
};

}  // namespace arty

#endif  // COLLISION_RENDERING_SYSTEM_HPP
