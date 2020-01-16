#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>

using namespace arty;

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow;
  Ptr<Keyboard> keyboard = window_impl->provideKeyboard();
  Ptr<Window> window(window_impl);
  Ptr<Blackboard> board(new Blackboard);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());

  Engine engine;
  engine.set_board(board).set_window(window).set_keyboard(keyboard).add_system(
      Ptr<System>(new DebugHidSystem(window, textRenderer)));

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
