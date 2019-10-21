#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <arty/core/ecs.h>
#include <arty/core/window.h>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

namespace arty {
class OpenGlWindow : public Window {
 private:
  GLFWwindow* _window;

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

  bool keyHasBeenPressed(Key key) override;

  int width() override;

  int height() override;
};
}  // namespace arty

#endif
