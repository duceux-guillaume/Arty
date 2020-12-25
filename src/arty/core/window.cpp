#include <arty/core/window.hpp>

namespace arty {

WindowMode WindowMode::FullScreen() {
  WindowMode mode;
  mode._type = FULLSCREEN;
  mode._width = -1;
  mode._height = -1;
  return mode;
}

}  // namespace arty