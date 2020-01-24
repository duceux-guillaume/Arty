#include <GL/glew.h>

#include <arty/ext/glfw/glfw_window.hpp>

namespace arty {

static void GLAPIENTRY MessageCallback(GLenum /*source*/, GLenum type,
                                       GLuint /*id*/, GLenum severity,
                                       GLsizei /*length*/,
                                       const GLchar* message,
                                       const void* /*userParam*/) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
}

Ptr<Keyboard> _keyboard = std::make_shared<Keyboard>();
static void keyboardCallback(GLFWwindow* /*window*/, int key, int /*scancode*/,
                             int action, int /*mod*/) {
  _keyboard->process(static_cast<Keyboard::Key>(key),
                     static_cast<Keyboard::Action>(action));
}

Result GlfwWindow::init() {
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
  auto monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
  _window =
      glfwCreateWindow(mode->width, mode->height, "BareMetal", monitor, NULL);
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
  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Set the mouse at the center of the screen
  glfwPollEvents();
  glfwSetCursorPos(_window, 1024 / 2, 768 / 2);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  // During init, enable debug output
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);

  (void)glfwSetKeyCallback(_window, keyboardCallback);

  return ok();
}

void GlfwWindow::clear() {
  // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
  // flickering, so it's there nonetheless.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlfwWindow::swapBuffer() {
  // Swap buffers
  glfwSwapBuffers(_window);
  glfwPollEvents();
}

bool GlfwWindow::isOk() {
  return glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(_window) == 0;
}

void GlfwWindow::close() {
  // Close OpenGL window and terminate GLFW
  glfwTerminate();
}

double GlfwWindow::getTime() { return glfwGetTime(); }

int GlfwWindow::width() {
  int width, height;
  glfwGetWindowSize(_window, &width, &height);
  return width;
}

int GlfwWindow::height() {
  int width, height;
  glfwGetWindowSize(_window, &width, &height);
  return height;
}

Ptr<Keyboard> GlfwWindow::provideKeyboard() { return _keyboard; }

Ptr<Mouse> GlfwWindow::provideMouse() {
  return Ptr<Mouse>(new GlfwMouse(this));
}

Mouse::position_type GlfwMouse::position() const {
  assert(_parent);
  auto window = _parent->window();
  if (!window) {
    return position_type();
  }
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  x -= width / 2.;
  y -= height / 2.;
  return position_type(x, y);
}

void GlfwMouse::set(const Mouse::position_type& pos) {
  assert(_parent);
  auto window = _parent->window();
  if (!window) {
    return;
  }
  glfwSetCursorPos(window, pos.x(), pos.y());
}

}  // namespace arty
