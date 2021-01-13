#ifndef ARTY_IMPL_GUI_SYSTEM_HPP
#define ARTY_IMPL_GUI_SYSTEM_HPP

#include <arty/core/renderer.hpp>
#include <arty/core/system.hpp>
#include <arty/core/window.hpp>

namespace arty {

class GuiSystem : public System {
 private:
  Ptr<Window> _window;
  Ptr<Renderer> _renderer;

  // System interface
 public:
  Result process(const Ptr<Memory>& board);
  Result init(const Ptr<Memory>& board);
  void release();

 public:
  GuiSystem(Ptr<Window> window, Ptr<Renderer> renderer)
      : _window(window), _renderer(renderer) {}
};

struct ButtonBuilder;

struct Button {
  Text text;
  Rectangle rect;

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
    button.text.position = Text::position_t(x, y);
    // button.rect.position = button.text.position;
    button.rect.halfLength.x() = button.text.content.size();
    return *this;
  }

  ButtonBuilder& size(unsigned s) {
    button.text.size = s;
    button.rect.halfLength.y() = s;
    return *this;
  }
};

class GuiFactory {
 private:
  std::vector<Button> _buttons;

 private:
  GuiFactory() = default;

 public:
  static GuiFactory make() { return GuiFactory(); }

  GuiFactory& add(Button button);

  void build(Ptr<Memory> const& mem);
};

}  // namespace arty

#endif  // ARTY_IMPL_GUI_SYSTEM_HPP