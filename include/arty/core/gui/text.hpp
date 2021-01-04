#ifndef ARTY_CORE_GUI_TEXT_HPP
#define ARTY_CORE_GUI_TEXT_HPP

#include <arty/core/math.hpp>

namespace arty {

struct Text {
  std::string content;
  Vec2u position;
  unsigned size;

  Text() : content(), position(0, 0), size(10) {}
};

}  // namespace arty

#endif  // !ARTY_CORE_GUI_TEXT_HPP
