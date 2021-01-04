#include <arty/impl/gui/gui_system.hpp>

namespace arty {

ButtonBuilder Button::builder() { return ButtonBuilder(); }

Result GuiSystem::process(Ptr<Memory> const& board) {
  if (board->count<Text>()) {  // Text
    auto work = [=](Entity const& /*e*/, Text const& t) -> Result {
      _renderer->draw(t);
      return ok();
    };
    board->process<Text>(work);
  }
  return ok();
}

Result GuiSystem::init(Ptr<Memory> const& board) {
  return_if_error(_renderer->loadFont("../models/Holstein.DDS"));

  for (auto const& button : _buttons) {
    auto e = board->createEntity("button");
    board->write(e, button.text);
  }

  return ok();
}

void GuiSystem::release() { _renderer->release(); }

GuiSystem& GuiSystem::add(Button button) {
  _buttons.push_back(button);
  return *this;
}
}  // namespace arty