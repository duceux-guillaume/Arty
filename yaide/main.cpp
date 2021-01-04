#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/gui/gui_system.hpp>

using namespace arty;

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow(WindowMode::FullScreen(), "YAIDE");
  Ptr<Window> window(window_impl);
  Ptr<Memory> board(new Memory);
  Ptr<Renderer> renderer(new Renderer(Ptr<ITextRenderer>(new GlTextRenderer)));

  Ptr<GuiSystem> gui(new GuiSystem(window, renderer));
  (*gui)
      .add(Button::builder().text("Button1").position(0, 0).size(10).build())
      .add(Button::builder().text("Button2").position(100, 0).size(10).build());

  Engine engine;
  engine.setBoard(board).setWindow(window).addSystem(gui);

  std::cout << "START: " << engine.start().message() << std::endl;
  std::cout << "RUN: " << engine.run().message() << std::endl;
  return 0;
}