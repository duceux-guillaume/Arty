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
  c.position = center;
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
  c.position = center;
  c.squaredRadius = squaredDistanceMin;
  return c;
}

Vec3f CollisionDetection::computeCenter(const Mesh& mesh) {
  Vec3f center;
  for (auto const& pt : mesh.vertices) {
    center += pt;
  }
  center /= mesh.vertices.size();
  return center;
}

Collision CollisionDetection::detect(const Mesh& mesh1, const Transform& tf1,
                                     const Mesh& mesh2, const Transform& tf2) {
  Collision col;
  col.exist = false;

  Sphere s1 = computeOuterCircle(mesh1);
  Sphere s2 = computeOuterCircle(mesh2);
  auto center1 = tf1 * s1.position;
  auto center2 = tf2 * s2.position;
  double dist = (center1 - center2).normsqr();
  std::cout << "dist " << dist << std::endl;
  std::cout << "vs " << s1.squaredRadius + s2.squaredRadius << std::endl;
  if (dist > s1.squaredRadius + s2.squaredRadius) {
    return col;
  }
  col.exist = true;
  col.shape = Shape3f::edge(center1, center2);
  return col;
}

}  // namespace arty
