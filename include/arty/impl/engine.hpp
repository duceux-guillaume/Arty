#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <arty/core/input.h>
#include <arty/core/result.h>
#include <arty/core/window.h>

#include <arty/core/system.hpp>

namespace arty {

class Engine {
 public:
  Engine() : _window(), _systems(), _state() {}

  Engine& setWindow(Ptr<Window> const& ptr);

  Engine& setKeyboard(Ptr<Keyboard> const& ptr);

  Engine& addSystem(Ptr<System> const& system);

  Engine& setBoard(Ptr<Memory> const& board);

  Result start();

  Result step();

  Result run();

  void stop();

  Ptr<Memory> board() const { return _state; }

 private:
  Ptr<Window> _window;
  std::vector<Ptr<System>> _systems;
  Ptr<Memory> _state;
  Ptr<Keyboard> _keyboard;
};

}  // namespace arty

#endif  // ENGINE_HPP
