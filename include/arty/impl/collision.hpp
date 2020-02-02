#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <arty/core/geometry.hpp>
#include <arty/impl/physics_system.hpp>

namespace arty {

class Collision {
 private:
  bool _exist;
  AABox3f _intersection;
  std::pair<Entity, Entity> _entities;

 public:
  Collision() : _exist(false), _intersection() {}
  Collision(AABox3f const& b) : _exist(true), _intersection(b) {}

  bool exist() const { return _exist; }
  AABox3f intersection() const { return _intersection; }
  bool hasVolume() const {
    return _intersection.halfLength().verify([](float v) { return v > 0; });
  }

  std::pair<Entity, Entity>& entities() { return _entities; }
  std::pair<Entity, Entity> const& entities() const { return _entities; }
  void set(Entity const& first, Entity const& second) {
    _entities.first = first;
    _entities.second = second;
  }
};

class CollisionDetection {
 public:
  Collision detect(Tf3f const& tf1, AABox3f const& b1, Tf3f const& tf2,
                   AABox3f const& b2);

 private:
};
/*
class CollisionSolver {
 public:
  void solve(Collision const& c, Particle* target, Particle other);

 private:
  Vec3f argmin(const Vec3f& tf) const;
};
*/
}  // namespace arty

#endif  // COLLISION_HPP
