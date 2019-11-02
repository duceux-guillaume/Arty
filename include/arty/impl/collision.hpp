#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <arty/core/math.h>

#include <arty/core/mesh.hpp>

namespace arty {

struct Circle {
  Vec3f position;
  float squaredRadius;
};

class CollisionDetection {
 public:
  Circle computeOuterCircle(Mesh const& mesh);
  Circle computeInnerCircle(Mesh const& mesh);
  Vec3f computeCenter(Mesh const& mesh);

 private:
};

}  // namespace arty

#endif  // COLLISION_HPP
