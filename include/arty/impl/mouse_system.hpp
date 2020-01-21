#ifndef MOUSE_SYSTEM_HPP
#define MOUSE_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/core/window.hpp>

namespace arty {

class MouseSystem : public System {
 public:
  MouseSystem(Ptr<Window> const &w) : _window(w) {}

 private:
  Ptr<Window> _window;

  // System interface
 public:
  Result process(const Ptr<Memory> &board);
  Result init(const Ptr<Memory> &board);
  void release();
};

}  // namespace arty

#endif  // MOUSE_SYSTEM_HPP
