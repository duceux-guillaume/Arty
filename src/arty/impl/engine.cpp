#include <arty/impl/engine.h>

namespace arty {

Engine &Engine::set_window(const Ptr<Window> &ptr) {
  _window = ptr;
  return *this;
}

Engine &Engine::add_system(const Ptr<System> &system) {
  _systems.push_back(system);
  return *this;
}

Result Engine::start() {
  Result r = _window->init();
  check_result(r);
  for (auto const &system : _systems) {
    r = system->init(_state);
    check_result(r);
  }
  return ok();
}

Result Engine::step() {
  Result res;
  _window->clear();
  for (auto system : _systems) {
    if (system) {
      res = system->process(_state);
      check_result(res);
    } else {
      return error("Null ptr system");
    }
  }
  _window->swapBuffer();
  return ok();
}

Result Engine::run() {
  Result res;
  size_t count = 0;
  do {
    res = step();
    count++;
  } while (res && _window->isOk() && count < 10000);
  return res;
}

void Engine::stop() {
  for (auto const &system : _systems) {
    system->release();
  }
  _window->close();
}

}  // namespace arty
