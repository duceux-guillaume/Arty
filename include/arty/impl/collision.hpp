#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <arty/core/math.h>

#include <arty/core/geometry.hpp>
#include <arty/core/mesh.hpp>
#include <variant>

namespace arty {

class CollisionDetection {
 public:
  Sphere computeOuterCircle(Mesh const& mesh);
  Sphere computeInnerCircle(Mesh const& mesh);
  Vec3f computeCenter(Mesh const& mesh);

 private:
};

}  // namespace arty

#endif  // COLLISION_HPP
