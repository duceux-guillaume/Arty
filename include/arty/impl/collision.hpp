#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <arty/core/math.h>

#include <arty/core/geometry.hpp>
#include <arty/core/memory.hpp>
#include <arty/core/mesh.hpp>

namespace arty {

struct Collision {
  bool exist;
  Shape3f shape;
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
  Box computeAxisAlignedBoundingBox(Mesh const& mesh);

 private:
};

}  // namespace arty

#endif  // COLLISION_HPP
