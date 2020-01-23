#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/physics.hpp>

namespace arty {

class PhysicsSystem : public System {
 public:
  static constexpr const char* OUTPUT = "transform";
  static constexpr const char* INPUT = "physics";

  PhysicsSystem(WorldPhysics const& world) : _world(world), _solver() {}

 private:
  WorldPhysics _world;
  PhysicsSolver _solver;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
};

}  // namespace arty

#endif  // PHYSICS_SYSTEM_HPP
