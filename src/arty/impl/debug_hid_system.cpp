#include <arty/impl/debug_hid_system.hpp>
#include <cmath>

namespace arty {

Result DebugHidSystem::process(Ptr<Memory> const& /*board*/) {
  static double lastTime = _window->getTime();
  double currentTime = _window->getTime();
  double deltaTime = currentTime - lastTime;
  lastTime = currentTime;
  std::string fps =
      std::to_string(std::round(100. / deltaTime) / 100.) + std::string(" fps");
  Text text;
  text.content = fps;
  text.position = Text::position_t(10, 10);
  text.size = 20;
  _renderer->draw(text);
  return ok();
}

Result DebugHidSystem::init(Ptr<Memory> const& /*board*/) {
  return _renderer->loadTextTexture("../models/Holstein.DDS");
}

void DebugHidSystem::release() { _renderer->release(); }

}  // namespace arty
