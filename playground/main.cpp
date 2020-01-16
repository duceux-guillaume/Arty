#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>

using namespace arty;

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow;
  Ptr<Keyboard> keyboard = window_impl->provideKeyboard();
  Ptr<Window> window(window_impl);
  Ptr<Blackboard> board(new Blackboard);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());
  Ptr<IShapeRenderer> shapeRenderer(new GlShapeRenderer());

  Engine engine;
  engine.setBoard(board)
      .setWindow(window)
      .setKeyboard(keyboard)
      .addSystem(Ptr<System>(new DebugHidSystem(window, textRenderer)))
      .addSystem(Ptr<FixedCameraSystem>(new FixedCameraSystem(window)))
      .addSystem(
          Ptr<HitBoxRenderingSystem>(new HitBoxRenderingSystem(shapeRenderer)));

  auto cube = board->createEntity("cube");
  board->set(cube, HitBoxRenderingSystem::DRAW_PROP,
             Box(Vec3f(0.f, 0.f, 0.f), Vec3f(1.f, 1.f, 1.f)));
  board->set(cube, "transform", Vec3f(0.f, 0.f, 0.f));
  /*
    auto floor = board->createEntity("floor");
    board->set(floor, HitBoxRenderingSystem::DRAW_PROP,
               Box(Vec3f(0.f, 0.f, -2.f), Vec3f(5.f, 5.f, 1.f)));
    board->set(floor, "transform", Vec3f(0.f, 0.f, -2.f));
  */
  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
