#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/physics.hpp>

namespace arty {

using CollisionArray = std::vector<Collision>;

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

class PhysicsSystem : public System {
 public:
  Result process(const Ptr<Memory>& board) override;
  Result integrateMotion(Ptr<Memory> const& mem) const;
  Result resolveCollision(Ptr<Memory> const& mem) const;
  Result detectCollision(Ptr<Memory> const& mem) const;
};

}  // namespace arty

#endif  // PHYSICS_SYSTEM_HPP
