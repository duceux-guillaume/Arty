#include <arty/impl/collision.hpp>
#include <numeric>

namespace arty {

Collision CollisionDetection::detect(const Tf3f& tf1, const AABox3f& b1,
                                     const Tf3f& tf2, const AABox3f& b2) {
  AABox3f rb1 = b1.move(tf1);
  AABox3f rb2 = b2.move(tf2);
  auto intersection = rb1.intersection(rb2);
  if (intersection.empty()) {
    return Collision();
  }
  return Collision(intersection.value());
}

void CollisionSolver::solve(const Collision& c, Physics* target,
                            Physics other) {
  if (!target->dynamic) {
    return;
  }
  Vec3f dir = argmin(c.intersection().halfLength());
  if (!other.dynamic) {
    dir *= 2.f;
  }
  auto distance =
      (other.position.translation() - target->position.translation()).normsqr();
  auto distance2 =
      (other.position.translation() - (target->position.translation() + dir))
          .normsqr();
  if (distance2 > distance) {
    target->position.translation() += dir;
  }
}

Vec3f CollisionSolver::argmin(const Vec3f& v) const {
  if (v.x() <= v.y() && v.x() <= v.z()) {
    return Vec3f{v.x(), 0.f, 0.f};
  }
  if (v.y() <= v.x() && v.y() <= v.z()) {
    return Vec3f{0., v.y(), 0.f};
  }
  return Vec3f{0., 0.f, v.z()};
}

}  // namespace arty
