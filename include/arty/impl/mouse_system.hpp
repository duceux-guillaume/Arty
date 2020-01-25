#ifndef MOUSE_SYSTEM_HPP
#define MOUSE_SYSTEM_HPP

#include <arty/core/input.hpp>
#include <arty/core/system.hpp>
#include <arty/impl/camera_system.hpp>

namespace arty {

class MouseSystem : public System {
 public:
  static constexpr const char* INPUT_1 = "transform";
  static constexpr const char* INPUT_2 = "hitbox";
  static constexpr const char* OUTPUT = "selected";

  Result process(Ptr<Memory> const& mem, Ptr<Keyboard> const& /*keyboard*/,
                 Ptr<Mouse> const& mouse);
};

}  // namespace arty

#endif  // MOUSE_SYSTEM_HPP
