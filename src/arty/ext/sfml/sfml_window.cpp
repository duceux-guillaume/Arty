#include <arty/ext/sfml/sfml_window.hpp>

namespace arty {

SfmlWindow::~SfmlWindow() {
  if (_window) {
    delete _window;
    _window = nullptr;
  }
}

Result SfmlWindow::init() {
  _window = new sf::Window;
  _window->create(sf::VideoMode(800, 600), "My window");
  return ok();
}

bool SfmlWindow::isOk() const { return _window && _window->isOpen(); }

bool SfmlWindow::isVisible() const { return isOk(); }

void SfmlWindow::swapBuffer() {
  assert(_window);
  // check all the window's events that were triggered since the last iteration
  // of the loop
  sf::Event event;
  while (_window->pollEvent(event)) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed) _window->close();
  }
}

int SfmlWindow::width() const {
  assert(_window);
  return _window->getSize().x;
}

int SfmlWindow::height() const {
  assert(_window);
  assert(_window);
  return _window->getSize().y;
}

}  // namespace arty