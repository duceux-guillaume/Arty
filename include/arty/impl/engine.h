#ifndef ENGINE_H
#define ENGINE_H

#include <arty/core/ecs.h>
#include <arty/core/result.h>
#include <arty/core/window.h>

namespace arty {

class Engine {
 public:
  Engine(Ptr<Window> const& ptr) : _window(ptr) {}

  Result start() { return _window->init(); }

  Result step() {
    _window->clear();
    std::cout << "Do stuff someday" << std::endl;
    _window->swapBuffer();
    return OK;
  }

  Result run() {
    Result res;
    do {
      res = step();
    } while (res && _window->isOk());
    return res;
  }

  void stop() { _window->close(); }

 private:
  Ptr<Window> _window;
};

}  // namespace arty

#endif  // ENGINE_H
