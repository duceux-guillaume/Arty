#include <arty/core/window.hpp>

namespace arty {

WindowMode WindowMode::FullScreen() {
  WindowMode mode;
  mode._type = FULLSCREEN;
  mode._width = -1;
  mode._height = -1;
  return mode;
}

WindowMode WindowMode::Windowed(int w, int h) {
  WindowMode mode;
  mode._type = WINDOWED;
  mode._width = w;
  mode._height = h;
  return mode;
}

}  // namespace arty