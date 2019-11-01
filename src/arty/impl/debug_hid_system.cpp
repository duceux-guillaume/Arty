#include <arty/impl/debug_hid_system.hpp>

namespace arty {

Result DebugHidSystem::process(Ptr<Blackboard> const& /*board*/) {
  static double lastTime = _window->getTime();
  double currentTime = _window->getTime();
  double deltaTime = currentTime - lastTime;
  lastTime = currentTime;
  std::string fps =
      std::to_string(std::round(100. / deltaTime) / 100.) + std::string(" fps");
  _renderer->printText2D(fps, 10, 10, 20);
  return ok();
}

Result DebugHidSystem::init(Ptr<Blackboard> const& /*board*/) {
  return _renderer->initText2D("../models/Holstein.DDS");
}

void DebugHidSystem::release() { _renderer->cleanupText2D(); }

}  // namespace arty
