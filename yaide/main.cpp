/*
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/gui/gui_system.hpp>

using namespace arty;

int main(void) {
  GlfwWindow* window_impl =
      new GlfwWindow(WindowMode::Windowed(800, 600), "YAIDE");
  Ptr<Window> window(window_impl);
  Ptr<Memory> board(new Memory);
  Ptr<Renderer> renderer(new Renderer(Ptr<IRenderer2D>(new GlRenderer2D)));

  Ptr<GuiSystem> gui(new GuiSystem(window, renderer));

  GuiFactory::make()
      .add(Button::builder().text("File").position(0, 0).size(10).build())
      .add(Button::builder().text("Edit").position(100, 0).size(10).build())
      .build(board);

  Engine engine;
  engine.setBoard(board).setWindow(window).addSystem(gui);

  std::cout << "START: " << engine.start().message() << std::endl;
  std::cout << "RUN: " << engine.run().message() << std::endl;
  return 0;
}
*/

// Headers
#include <GL/glew.h>

#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_renderer.hpp>
#include <arty/ext/sfml/sfml_window.hpp>

using namespace arty;

int main() {
  SfmlWindow* window_impl =
      new SfmlWindow(WindowMode::Windowed(800, 600), "YAIDE");
  Ptr<Window> window(window_impl);
  window->init();

  Ptr<Memory> mem(new Memory);

  auto but1 = mem->createEntity("button1");
  mem->write(but1, Rectangle(0.f, 0.95f, 1.f, 0.05f, 0.2f, 0.2f, 0.2f, 1.f));

  auto but2 = mem->createEntity("button1");
  mem->write(but2, Rectangle(-0.2f, -0.2f, 0.3f, 0.3f, 0.f, 1.f, 0.f, 1.f));

  Ptr<GlRenderer> renderer(new GlRenderer);
  Ptr<GlRenderer2D> text_renderer(new GlRenderer2D);
  text_renderer->loadTextTexture("../models/Holstein.DDS");

  while (window->isOk()) {
    window->clear();

    if (mem->count<Rectangle>()) {
      auto work = [=](Entity const& e, Rectangle const& r) -> Result {
        if (renderer->has(e)) {
          renderer->draw(e);
        } else {
          renderer->create(e, r);
        }
        return ok();
      };
      mem->process<Rectangle>(work);
    }
    text_renderer->draw(Text("File", Text::position_t(10, 10), 20));

    // Swap buffers
    window->refresh();
  }

  renderer->release();
  return 0;
}