#ifndef ARTY_CORE_GUI_RECTANGLE_HPP
#define ARTY_CORE_GUI_RECTANGLE_HPP

#include <arty/core/math.hpp>

namespace arty {

struct Rectangle {
  using position_t = Vec2f;
  using length_t = Vec2f;
  using color_t = Vec4f;

  position_t position;
  length_t halfLength;
  color_t fillColor;

  Rectangle() {}

  Rectangle(position_t const& p, length_t const& l, color_t const& c)
      : position(p), halfLength(l), fillColor(c) {}

  Rectangle(float x, float y, float hw, float hh, float r, float g, float b,
            float a)
      : Rectangle(position_t(x, y), length_t(hw, hh), color_t(r, g, b, a)) {}

  position_t topLeft() const {
    return position_t(position.x() - halfLength.x(),
                      position.y() + halfLength.y());
  }

  position_t topRight() const {
    return position_t(position.x() + halfLength.x(),
                      position.y() + halfLength.y());
  }

  position_t bottomRight() const {
    return position_t(position.x() + halfLength.x(),
                      position.y() - halfLength.y());
  }

  position_t bottomLeft() const {
    return position_t(position.x() - halfLength.x(),
                      position.y() - halfLength.y());
  }
};

}  // namespace arty

#endif  // !ARTY_CORE_GUI_RECTANGLE_HPP