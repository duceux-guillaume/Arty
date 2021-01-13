#include <arty/impl/gui/gui_system.hpp>

namespace arty {

ButtonBuilder Button::builder() { return ButtonBuilder(); }

Result GuiSystem::process(Ptr<Memory> const& board) {
  if (board->count<Button>()) {  // Text
    auto work = [=](Entity const& /*e*/, Button const& b) -> Result {
      _renderer->draw(b.rect);
      //_renderer->draw(b.text);
      return ok();
    };
    board->process<Button>(work);
  }
  return ok();
}

Result GuiSystem::init(Ptr<Memory> const& /*board*/) {
  return _renderer->loadTextTexture("../models/Holstein.DDS");
}

void GuiSystem::release() { _renderer->release(); }

GuiFactory& GuiFactory::add(Button button) {
  _buttons.push_back(button);
  return *this;
}

void GuiFactory::build(Ptr<Memory> const& mem) {
  for (auto const& button : _buttons) {
    auto e = mem->createEntity("button");
    mem->write(e, button);
  }
  _buttons.clear();
}

}  // namespace arty