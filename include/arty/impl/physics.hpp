#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <arty/core/geometry.hpp>
#include <vector>

namespace arty {

struct WorldPhysics {
  float gravity_strengh;
  float air_friction;
  float ground_friction;
  float min_ground_level;
};

class Physics {
 public:
  using number_type = float;
  using vector_type = Vec3<number_type>;
  using transform_type = Transform<number_type, 3>;

  Physics() : Physics(Tf3f(), 0.f) {}
  Physics(Tf3f const& pos, float mass)
      : _position(pos),
        _velocity(),
        _acceleration(),
        _mass(mass),
        _dynamic(true) {
    if (mass <= 0.f) {
      _mass = 0.f;
      _dynamic = false;
    }
  }

  bool isStatic() const { return !_dynamic; }

  transform_type const& position() const { return _position; }
  transform_type const& velocity() const { return _velocity; }
  transform_type const& acceleration() const { return _acceleration; }
  transform_type& acceleration() { return _acceleration; }

  void move(transform_type const& mvmt) { _position *= mvmt; }
  void move(vector_type const& mvmt) { _position += mvmt; }

  void impulse(vector_type const& imp) { _velocity += imp; }
  void stop() { _velocity = transform_type(); }

  void apply(vector_type const& force, vector_type const& pos) {
    _forces_dir.push_back(force);
    _forces_pos.push_back(pos);
  }

  void apply(vector_type const& force) {
    _forces_dir.push_back(force);
    _forces_pos.push_back(position().translation());
  }

  void flush() {
    _forces_dir.clear();
    _forces_pos.clear();
  }

  number_type kinetic() const {
    return _mass * std::pow(_velocity.translation().norm(), 2.);
  }

  std::vector<vector_type> const& forces() const { return _forces_dir; }

  number_type mass() const { return _mass; }

 private:
  transform_type _position;
  transform_type _velocity;
  transform_type _acceleration;
  number_type _mass;
  bool _dynamic;
  std::vector<vector_type> _forces_dir;
  std::vector<vector_type> _forces_pos;
};

class PhysicsSolver {
 public:
  void update(WorldPhysics const& world, Physics* phy);

 private:
};

}  // namespace arty

#endif  // PHYSICS_HPP
