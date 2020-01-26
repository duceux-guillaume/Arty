#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP

#include <GLFW/glfw3.h>

#include <arty/core/input.hpp>
#include <arty/core/window.hpp>

namespace arty {

class GlfwWindow;

class GlfwMouse : public Mouse {
 private:
  GlfwWindow* _parent;

 public:
  GlfwMouse(GlfwWindow* win) : _parent(win) {}

  position_type position() const override;
  void set(position_type const& pos) override;

  void callback(GLFWwindow*, int button, int action, int) {
    process(button, static_cast<Action>(action));
  }
};

class GlfwWindow : public Window {
 private:
  GLFWwindow* _window;

 public:
  GlfwWindow() : _window() {}

  GLFWwindow* window() const { return _window; }

  // Window interface
 public:
  Result init() override;

  void clear() override;

  void swapBuffer() override;

  bool isOk() override;

  void close() override;

  double getTime() override;

  int width() override;

  int height() override;

  Ptr<Keyboard> provideKeyboard();

  Ptr<Mouse> provideMouse();
};
}  // namespace arty

#endif  // GLFW_WINDOW_HPP
