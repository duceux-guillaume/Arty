#ifndef ARTY_CORE_GUI_TEXT_HPP
#define ARTY_CORE_GUI_TEXT_HPP

#include <arty/core/math.hpp>

namespace arty {

struct Text {
  using position_t = Vec2i;

  std::string content;
  position_t position;
  unsigned size;

  Text() : content(), position(0, 0), size(10) {}
  Text(std::string const& t, position_t p, unsigned s)
      : content(t), position(p), size(s) {}
};

}  // namespace arty

#endif  // !ARTY_CORE_GUI_TEXT_HPP
