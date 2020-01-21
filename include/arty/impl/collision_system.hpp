#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/collision.hpp>

namespace arty {

class CollisionDetectionSystem : public System {
 public:
  static constexpr const char* INPUT_1 = "transform";
  static constexpr const char* INPUT_2 = "hitbox";
  static constexpr const char* OUTPUT = "collision";

 private:
  CollisionDetection _collision;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;
};

}  // namespace arty

#endif  // COLLISION_SYSTEM_HPP
