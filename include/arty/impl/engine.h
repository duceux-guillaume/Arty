#ifndef ENGINE_H
#define ENGINE_H

#include <arty/core/input.h>
#include <arty/core/result.h>
#include <arty/core/window.h>

#include <arty/core/system.hpp>

namespace arty {

class Engine {
 public:
  Engine() : _window(), _systems(), _state() {}

  Engine& set_window(Ptr<Window> const& ptr);

  Engine& set_keyboard(Ptr<Keyboard> const& ptr);

  Engine& add_system(Ptr<System> const& system);

  Engine& set_board(Ptr<Blackboard> const& board);

  Result start();

  Result step();

  Result run();

  void stop();

  Ptr<Blackboard> board() const { return _state; }

 private:
  Ptr<Window> _window;
  std::vector<Ptr<System>> _systems;
  Ptr<Blackboard> _state;
  Ptr<Keyboard> _keyboard;
};

}  // namespace arty

#endif  // ENGINE_H
