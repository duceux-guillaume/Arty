#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/collision.hpp>

namespace arty {
class CollisionSystem : public System {
 public:
 private:
  CollisionDetection _collision;
  // System interface
 public:
  Result process(const Ptr<Blackboard> &board) override;
  Result init(const Ptr<Blackboard> &board) override;
  void release() override;
};
}  // namespace arty

#endif  // COLLISION_SYSTEM_HPP
