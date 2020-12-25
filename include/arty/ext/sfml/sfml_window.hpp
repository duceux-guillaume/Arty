#ifndef ARTY_EXT_SFML_WINDOW_HPP
#define ARTY_EXT_SFML_WINDOW_HPP

#include <SFML/Window.hpp>
#include <arty/core/window.hpp>

namespace arty {

class SfmlWindow : public Window {
 private:
  sf::Window* _window;

 public:
  SfmlWindow(WindowMode mode, std::string const name)
      : Window(mode, name), _window(nullptr) {}
  ~SfmlWindow();

  // Window interface
 public:
  Result init() override;

  void clear() override;

  void refresh() override;

  bool isOk() const override;

  void close() override;

  double getTime() override { return 0.; }

  int width() const override;

  int height() const override;

  bool isVisible() const override;
};

}  // namespace arty

#endif  // ARTY_EXT_SFML_WINDOW_HPP