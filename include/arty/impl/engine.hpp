#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <arty/core/input.hpp>
#include <arty/core/result.hpp>
#include <arty/core/system.hpp>
#include <arty/core/window.hpp>

namespace arty {

class Engine {
 public:
  Engine() : _window(), _systems(), _state(), _inputs(new InputManager) {}

  Engine& setWindow(Ptr<Window> const& ptr);

  Engine& setKeyboard(Ptr<Keyboard> const& ptr);

  Engine& setMouse(Ptr<Mouse> const& ptr);

  Engine& addSystem(Ptr<System> const& system);

  template <class DerivedSystem>
  Engine& makeSystem() {
    return this->addSystem(Ptr<DerivedSystem>(new DerivedSystem));
  }

  template <class DerivedSystem, typename... Args>
  Engine& makeSystem(Args... args) {
    return this->addSystem(Ptr<DerivedSystem>(new DerivedSystem(args...)));
  }

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
  Ptr<InputManager> _inputs;
};

}  // namespace arty

#endif  // ENGINE_HPP
