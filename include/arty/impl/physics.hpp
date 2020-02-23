#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/memory.hpp>
#include <arty/core/number.hpp>
#include <utility>
#include <vector>

namespace arty {
using number_t = double;
using vector_t = Vec3<number_t>;

class Collision {
 private:
  bool _exist;
  std::pair<Entity, Entity> _entities;
  vector_t _normal;
  vector_t _center;
  number_t _penetration;

 public:
  Collision() : _exist(false) {}
  Collision(vector_t const& normal, vector_t const& center,
            number_t penetration)
      : _exist(true),
        _normal(normal),
        _center(center),
        _penetration(penetration) {}

  bool exist() const { return _exist; }
  vector_t const& center() const { return _center; }
  vector_t const& normal() const { return _normal; }
  number_t const& penetration() const { return _penetration; }

  std::pair<Entity, Entity>& entities() { return _entities; }
  std::pair<Entity, Entity> const& entities() const { return _entities; }
  void set(Entity const& first, Entity const& second) {
    _entities.first = first;
    _entities.second = second;
  }
};  // namespace arty

class Particle {
 public:
  vector_t position;
  vector_t velocity;
  vector_t gravity;
  vector_t forceaccu;
  number_t damping;
  number_t restitution;
  bool isStatic() { return _is_static; }
  void setMass(number_t m) {
    if (m == 0) {
      _is_static = true;
      _invmass = 0;
      gravity = vector_t();
    } else {
      _is_static = false;
      _invmass = 1 / m;
    }
  }
  number_t const& inverseMass() const { return _invmass; }

  Particle()
      : Particle(vector_t(), vector_t(), vector_t(0, 0, -10), vector_t(),
                 number_t(0.9), number_t(1), number_t(0.1)) {}

  Particle(vector_t p, vector_t v, vector_t g, vector_t f, number_t d,
           number_t m, number_t r)
      : position(p),
        velocity(v),
        gravity(g),
        forceaccu(f),
        damping(d),
        restitution(r),
        _invmass(1 / m),
        _is_static(false) {
    if (m == 0) {
      _is_static = true;
    }
  }

  Tf3f transform() const { return Tf3f(static_cast<Vec3f>(position)); }

 private:
  number_t _invmass;
  bool _is_static;
};

class Physics {
 public:
  void integrateMotion(Particle& p, double duration) const;
  Collision detectCollision(Tf3f const& tf1, AABox3f const& b1, Tf3f const& tf2,
                            AABox3f const& b2);
  void resolveVelocity(Collision const& c, Particle& p1, Particle& p2,
                       double duration) const;
  void resolvePenetration(Collision const& c, Particle& p1, Particle& p2) const;
  void resolve(Collision const& c, Particle& p1, Particle& p2,
               double duration) const;
  number_t separatingVelocity(const Collision& c, const Particle& p1,
                              const Particle& p2) const;
};

}  // namespace arty

#endif  // PHYSICS_HPP
