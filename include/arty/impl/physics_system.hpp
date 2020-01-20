#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <arty/core/system.hpp>

namespace arty {

struct WorldPhysics {
  float gravity_strengh;
  float air_friction;
  float ground_friction;
  float min_ground_level;
};

struct Physics {
  Transform velocity;
  Transform acceleration;
  float mass;
  bool dynamic;

  Physics() : Physics(0.f) {}
  Physics(float mass) : velocity(), acceleration(), mass(mass), dynamic(true) {
    if (mass <= 0.f) {
      dynamic = false;
    }
  }
};

class PhysicsSolver {
 public:
  void update(Transform* tf, Physics* phy, WorldPhysics const& world);

 private:
};

class PhysicsSystem : public System {
 public:
  static constexpr const char* INPUT_PROP = "physics";
  static constexpr const char* OUTPUT_PROP = "transform";

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
