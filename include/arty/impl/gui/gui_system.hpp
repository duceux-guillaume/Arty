#ifndef ARTY_IMPL_GUI_SYSTEM_HPP
#define ARTY_IMPL_GUI_SYSTEM_HPP

#include <arty/core/renderer.hpp>
#include <arty/core/system.hpp>
#include <arty/core/window.hpp>

namespace arty {

struct ButtonBuilder;

struct Button {
  Text text;

  static ButtonBuilder builder();
};

struct ButtonBuilder {
  Button button;

  ButtonBuilder() : button() {}

  Button build() const { return button; }

  ButtonBuilder& text(std::string const& text) {
    button.text.content = text;
    return *this;
  }

  ButtonBuilder& position(unsigned x, unsigned y) {
    button.text.position = Vec2u(x, y);
    return *this;
  }

  ButtonBuilder& size(unsigned s) {
    button.text.size = s;
    return *this;
  }
};

class GuiSystem : public System {
 private:
  Ptr<Window> _window;
  Ptr<Renderer> _renderer;
  std::vector<Button> _buttons;

  // System interface
 public:
  Result process(const Ptr<Memory>& board);
  Result init(const Ptr<Memory>& board);
  void release();

 public:
  GuiSystem(Ptr<Window> window, Ptr<Renderer> renderer)
      : _window(window), _renderer(renderer) {}

  GuiSystem& add(Button button);
};

}  // namespace arty

#endif  // ARTY_IMPL_GUI_SYSTEM_HPP