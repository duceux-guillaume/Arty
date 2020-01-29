#ifndef PLAYER_CONTROL_HPP
#define PLAYER_CONTROL_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/system.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <iostream>

namespace arty {

class PlayerControlSystem : public System {
 private:
  Event _up;

 public:
  // System interface
 public:
  Result process(const Ptr<Memory>& board,
                 Ptr<InputManager> const& inputs) override;
  Result init(const Ptr<Memory>& board,
              Ptr<InputManager> const& inputs) override;
  void release() override;
};

}  // namespace arty

#endif  // PLAYER_CONTROL_HPP
