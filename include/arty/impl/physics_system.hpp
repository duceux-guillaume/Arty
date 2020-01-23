#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/system.hpp>

namespace arty {

struct WorldPhysics {
  float gravity_strengh;
  float air_friction;
  float ground_friction;
  float min_ground_level;
};

struct Physics {
  Tf3f position;
  Tf3f velocity;
  Tf3f acceleration;
  float mass;
  bool dynamic;

  Physics() : Physics(Tf3f(), 0.f) {}
  Physics(Tf3f const& pos, float mass)
      : position(pos), velocity(), acceleration(), mass(mass), dynamic(true) {
    if (mass <= 0.f) {
      dynamic = false;
    }
  }
};

class PhysicsSolver {
 public:
  void update(Tf3f* tf, Physics* phy, WorldPhysics const& world);

 private:
};

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
