#include <arty/ext/opengl_window.h>

namespace arty {

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message,
                                       const void* userParam) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
}

Result OpenGlWindow::init() {
  // Initialise GLFW
  if (!glfwInit()) {
    return Result("Failed to initialize GLFW\n");
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 GL_TRUE);  // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  _window = glfwCreateWindow(1024, 768, "Bot Arena", NULL, NULL);
  if (_window == NULL) {
    glfwTerminate();
    return Result(
        "Failed to open GLFW window. If you have an Intel GPU, they are "
        "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
  }
  glfwMakeContextCurrent(_window);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    return Result("Failed to initialize GLEW\n");
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // During init, enable debug output
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);

  return ok();
}

void OpenGlWindow::clear() {
  // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
  // flickering, so it's there nonetheless.
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGlWindow::swapBuffer() {
  // Swap buffers
  glfwSwapBuffers(_window);
  glfwPollEvents();
}

bool OpenGlWindow::isOk() {
  return glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(_window) == 0;
}

void OpenGlWindow::close() {
  // Close OpenGL window and terminate GLFW
  glfwTerminate();
}

}  // namespace arty
