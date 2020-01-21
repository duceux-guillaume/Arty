#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <GLFW/glfw3.h>

#include <arty/core/input.hpp>
#include <arty/core/window.hpp>

namespace arty {

class GlfwWindow : public Window, public Keyboard {
 private:
  GLFWwindow* _window;

 public:
  GlfwWindow() : _window() {}

  // Window interface
 public:
  Result init() override;

  void clear() override;

  void swapBuffer() override;

  bool isOk() override;

  void close() override;

  CursorPosition getCursorPosition() override;

  void setCursorPosition(CursorPosition const& cursor) override;

  double getTime() override;

  int width() override;

  int height() override;

  Ptr<Keyboard> provideKeyboard();
};
}  // namespace arty

#endif  // GLFW_WINDOW_HPP
