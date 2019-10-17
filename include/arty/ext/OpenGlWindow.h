#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <stdio.h>
#include <stdlib.h>

#include <arty/core/ecs.h>
#include <arty/core/window.h>

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
  Result init() {
    // Initialise GLFW
    if (!glfwInit()) {
      getchar();
      return Result("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);  // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    _window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (_window == NULL) {
      getchar();
      glfwTerminate();
      return Result(
          "Failed to open GLFW window. If you have an Intel GPU, they are "
          "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    }
    glfwMakeContextCurrent(_window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
      getchar();
      glfwTerminate();
      return Result("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    return OK;
  }

  void clear() {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
    // flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void swapBuffer() {
    // Swap buffers
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  bool isOk() {
    return glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(_window) == 0;
  }

  void close() {
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
  }
};
}  // namespace arty

#endif
