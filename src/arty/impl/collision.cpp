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

Collision CollisionDetection::detect(const Mesh&, const Tf3f&, const Mesh&,
                                     const Tf3f&) {
  Collision col;
  /*
   col.exist = false;

   Sphere s1 = computeOuterCircle(mesh1);
   Sphere s2 = computeOuterCircle(mesh2);
   auto center1 = tf1 * s1.center;
   auto center2 = tf2 * s2.center;
   double dist = (center1 - center2).normsqr();
   if (dist > s1.squaredRadius + s2.squaredRadius) {
     return col;
   }
   AABox b1 = computeAxisAlignedBoundingBox(mesh1);
   AABox b2 = computeAxisAlignedBoundingBox(mesh2);
   double dx = std::abs(center1.x() - center2.x());
   double tx = b1._.x() + b2._.x();
   if (dx > tx) {
     return col;
   }
   double dy = std::abs(center1.y() - center2.y());
   double ty = b1._.y() + b2._.y();
   if (dy > ty) {
     return col;
   }
   double dz = std::abs(center1.z() - center2.z());
   double tz = b1._.z() + b2._.z();
   if (dz > tz) {
     return col;
   }

   col.exist = true;
   Vec3f z(tx - dx, ty - dy, tz - dz);
   Vec3f imp = (center1 + center2) * 0.5f;
   col.shape = Polygon3f::edge(imp, imp + z);
   */
  return col;
}

AABox3f CollisionDetection::computeAxisAlignedBoundingBox(const Mesh&) {
  AABox3f b;
  /*
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
  b._center = (maxval + minval) * 0.5f;
  b._ = (maxval - minval) * 0.5f;
  */
  return b;
}

Collision CollisionDetection::detect(const Tf3f& tf1, const AABox3f& b1,
                                     const Tf3f& tf2, const AABox3f& b2) {
  Collision col;
  col.exist = false;

  AABox3f rb1 = b1.move(tf1);
  AABox3f rb2 = b2.move(tf2);
  if (!rb1.intersect(rb2)) {
    return col;
  }

  col.exist = true;
  col.shape = Polygon3f::edge(rb1.center(), rb2.center());
  return col;
}

}  // namespace arty
