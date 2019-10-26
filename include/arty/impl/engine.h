#ifndef ENGINE_H
#define ENGINE_H

#include <arty/core/ecs.h>
#include <arty/core/property.h>
#include <arty/core/result.h>
#include <arty/core/window.h>

namespace arty {

class Engine {
 public:
  Engine() : _window(), _systems(), _state(new Blackboard) {}

  Engine& set_window(Ptr<Window> const& ptr);

  Engine& add_system(Ptr<System> const& system);

  Result start();

  Result step();

  Result run();

  void stop();

  Ptr<Blackboard> board() const { return _state; }

 private:
  Ptr<Window> _window;
  std::vector<Ptr<System>> _systems;
  Ptr<Blackboard> _state;
};

}  // namespace arty

#endif  // ENGINE_H
