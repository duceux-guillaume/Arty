#include <arty/impl/collision.hpp>
#include <numeric>

namespace arty {

Sphere CollisionDetection::computeOuterCircle(const Mesh& mesh) {
  Vec3f center = computeCenter(mesh);
  float squaredDistanceMax = 0.f;
  for (auto const& pt : mesh.vertices) {
    float distance2 = (center - pt).normsqr();
    if (distance2 > squaredDistanceMax) {
      squaredDistanceMax = distance2;
    }
  }
  Sphere c;
  c.center = center;
  c.squaredRadius = squaredDistanceMax;
  return c;
}

Sphere CollisionDetection::computeInnerCircle(const Mesh& mesh) {
  if (mesh.vertices.size() == 0) {
    return Sphere{Vec3f(), 0.f};
  }
  if (mesh.vertices.size() == 1) {
    return Sphere{mesh.vertices[0], 0.f};
  }

  Vec3f center = computeCenter(mesh);
  float squaredDistanceMin = std::numeric_limits<float>::max();
  /* FIXME
  auto ptIt = mesh.vertices.begin();
  auto nextIt = ptIt++;
  auto endIt = mesh.vertices.end();
  for (; nextIt != endIt; ++nextIt, ++ptIt) {
    float distance2 = squaredDistancePointToLine(center, *ptIt, *nextIt);
    if (distance2 < squaredDistanceMin) {
      squaredDistanceMin = distance2;
    }
  }*/
  Sphere c;
  c.center = center;
  c.squaredRadius = squaredDistanceMin;
  return c;
}

Vec3f CollisionDetection::computeCenter(const Mesh& mesh) {
  Vec3f maxval;
  static const float max = std::numeric_limits<float>::max();
  Vec3f minval(max, max, max);
  for (auto const& pt : mesh.vertices) {
    if (maxval.x() < pt.x()) {
      maxval.x() = pt.x();
    }
    if (maxval.y() < pt.y()) {
      maxval.y() = pt.y();
    }
    if (maxval.z() < pt.z()) {
      maxval.z() = pt.z();
    }
    if (minval.x() > pt.x()) {
      minval.x() = pt.x();
    }
    if (minval.y() > pt.y()) {
      minval.y() = pt.y();
    }
    if (minval.z() > pt.z()) {
      minval.z() = pt.z();
    }
  }
  return (maxval + minval) * 0.5f;
}

Collision CollisionDetection::detect(const Mesh& mesh1, const Transform& tf1,
                                     const Mesh& mesh2, const Transform& tf2) {
  Collision col;
  col.exist = false;

  Sphere s1 = computeOuterCircle(mesh1);
  Sphere s2 = computeOuterCircle(mesh2);
  auto center1 = tf1 * s1.center;
  auto center2 = tf2 * s2.center;
  double dist = (center1 - center2).normsqr();
  if (dist > s1.squaredRadius + s2.squaredRadius) {
    return col;
  }
  Box b1 = computeAxisAlignedBoundingBox(mesh1);
  Box b2 = computeAxisAlignedBoundingBox(mesh2);
  double dx = std::abs(center1.x() - center2.x());
  double tx = b1.halfLength.x() + b2.halfLength.x();
  if (dx > tx) {
    return col;
  }
  double dy = std::abs(center1.y() - center2.y());
  double ty = b1.halfLength.y() + b2.halfLength.y();
  if (dy > ty) {
    return col;
  }
  double dz = std::abs(center1.z() - center2.z());
  double tz = b1.halfLength.z() + b2.halfLength.z();
  if (dz > tz) {
    return col;
  }

  col.exist = true;
  Vec3f z(tx - dx, ty - dy, tz - dz);
  Vec3f imp = (center1 + center2) * 0.5f;
  col.shape = Shape3f::edge(imp, imp + z);
  return col;
}

Box CollisionDetection::computeAxisAlignedBoundingBox(const Mesh& mesh) {
  Box b;
  Vec3f maxval;
  static const float max = std::numeric_limits<float>::max();
  Vec3f minval(max, max, max);
  for (auto const& pt : mesh.vertices) {
    if (maxval.x() < pt.x()) {
      maxval.x() = pt.x();
    }
    if (maxval.z() < pt.z()) {
      maxval.z() = pt.z();
    }
    if (maxval.y() < pt.y()) {
      maxval.y() = pt.y();
    }
    if (minval.x() > pt.x()) {
      minval.x() = pt.x();
    }
    if (minval.y() > pt.y()) {
      minval.y() = pt.y();
    }
    if (minval.z() > pt.z()) {
      minval.z() = pt.z();
    }
  }
  b.center = (maxval + minval) * 0.5f;
  b.halfLength = (maxval - minval) * 0.5f;
  return b;
}

Collision CollisionDetection::detect(const Transform& tf1, const Box& b1,
                                     const Transform& tf2, const Box& b2) {
  Collision col;
  col.exist = false;

  Sphere s1(tf1.translation + b1.center, b1.halfLength.normsqr());
  Sphere s2(tf2.translation + b2.center, b2.halfLength.normsqr());
  if (!s1.intersect(s2)) {
    return col;
  }

  Box rb1(tf1.translation + b1.center, b1.halfLength);
  Box rb2(tf2.translation + b2.center, b2.halfLength);
  if (!rb1.intersect(rb2)) {
    return col;
  }

  col.exist = true;
  Vec3f imp = (rb1.center + rb2.center) * 0.5f;
  col.shape = Shape3f::edge(imp, imp + rb1.halfLength);
  return col;
}

}  // namespace arty
