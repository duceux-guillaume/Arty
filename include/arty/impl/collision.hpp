#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/math.hpp>
#include <arty/core/memory.hpp>
#include <arty/core/mesh.hpp>

namespace arty {

struct Collision {
  bool exist;
  Polygon3f shape;
  Entity e1;
  Entity e2;
};

class CollisionDetection {
 public:
  Sphere computeOuterCircle(Mesh const& mesh);
  Sphere computeInnerCircle(Mesh const& mesh);
  Vec3f computeCenter(Mesh const& mesh);
  Collision detect(Mesh const& mesh1, Transform const& tf1, Mesh const& mesh2,
                   Transform const& tf2);
  AABox3f computeAxisAlignedBoundingBox(Mesh const& mesh);
  Collision detect(Transform const& tf1, AABox3f const& b1,
                   Transform const& tf2, AABox3f const& b2);

 private:
};

}  // namespace arty

#endif  // COLLISION_HPP
