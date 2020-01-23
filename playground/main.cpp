#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <random>

using namespace arty;

void makeCube(std::string const& name, Tf3f const& pos, Vec3f const& length,
              float mass, Ptr<Memory> mem) {
  auto entity = mem->createEntity(name);
  mem->write(entity, HitBoxRenderingSystem::DRAW_PROP,
             AABox3f(Vec3f(0.f, 0.f, 0.f), length));
  mem->write(entity, PhysicsSystem::INPUT, Physics(pos, mass));
}

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow;
  Ptr<Keyboard> keyboard = window_impl->provideKeyboard();
  Ptr<Window> window(window_impl);
  Ptr<Memory> board(new Memory);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());
  Ptr<IShapeRenderer> shapeRenderer(new GlShapeRenderer());

  WorldPhysics world;
  world.gravity_strengh = 10.f;

  Engine engine;
  engine.setBoard(board)
      .setWindow(window)
      .setKeyboard(keyboard)
      .makeSystem<DebugHidSystem>(window, textRenderer)
      .makeSystem<FixedCameraSystem>(window)
      .makeSystem<HitBoxRenderingSystem>(shapeRenderer)
      .makeSystem<PhysicsSystem>(world)
      .makeSystem<CollisionDetectionSystem>()
      .makeSystem<CollisionRenderingSystem>(shapeRenderer)
      .makeSystem<CollisionSolverSystem>();

  makeCube("floor", Tf3f(Vec3f(6.f, 0.f, -5.f)), Vec3f(10.f, 10.f, 0.5f), 0.f,
           board);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> xdis(1.f, 11.f);
  std::uniform_real_distribution<> ydis(-5.f, 5.f);
  std::uniform_real_distribution<> zdis(0.f, 10.f);
  std::uniform_real_distribution<> lendis(0.1f, 2.f);
  for (int n = 0; n < 120; ++n) {
    makeCube("random", Tf3f(Vec3f(xdis(gen), ydis(gen), zdis(gen))),
             Vec3f(lendis(gen), lendis(gen), lendis(gen)), 1.f, board);
  }

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
