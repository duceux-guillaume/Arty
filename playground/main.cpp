#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/mouse_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <random>

using namespace arty;

void makeBrick(std::string const& name, Tf3f const& pos, Vec3f const& length,
               float mass, Ptr<Memory> mem) {
  auto entity = mem->createEntity(name);
  mem->write(entity, HitBoxRenderingSystem::DRAW_OBB, OBB3f(Tf3f(), length));
  mem->write(entity, PhysicsSystem::INPUT, Physics(pos, mass));
}

void makeWall(Vec3f min, Vec3f max, std::size_t counth, std::size_t countv,
              Ptr<Memory> mem) {
  auto stepz = (max.z() - min.z()) / countv;
  auto thickness = (max.y() - min.y()) / 2;
  int i = 0;
  for (float z = min.z(); z <= max.z(); z += stepz, ++i) {
    auto stepx = (max.x() - min.x()) / counth;
    float x = min.x();
    if (i & 1) {
      x += stepx / 2;
    }
    for (; x <= max.x(); x += stepx) {
      makeBrick("brick", Tf3f(Vec3f(x, thickness, z)),
                Vec3f(stepx / 2, thickness, stepz / 2), 0.f, mem);
    }
  }
}

class InitSystem : public System {
 private:
  Event _reset;

  void reset(Ptr<Memory> const& mem) {
    mem->clear();
    makeBrick("floor", Tf3f(), Vec3f(5.f, 5.f, 1.f), 0.f, mem);
    makeWall(Vec3f(-5.f, 0.f, 1.f), Vec3f(5.f, 0.2f, 5.f), 10, 4, mem);
  }

 public:
  InitSystem() : _reset("RESET") {}

  Result process(const Ptr<Memory>& mem,
                 Ptr<InputManager> const& inputs) override {
    if (inputs->pop(_reset)) {
      reset(mem);
    }
    return ok();
  }
  Result init(const Ptr<Memory>& mem,
              Ptr<InputManager> const& inputs) override {
    if (!inputs->attach(Keyboard::SPACE, Device::Action::PRESS, _reset)) {
      return error("couldn't register event");
    }
    reset(mem);
    return ok();
  }
};

class CursorRenderingSystem : public System {
 public:
  Result process(Ptr<Memory> const& mem);
  CursorRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
};

Result CursorRenderingSystem::process(const Ptr<Memory>& mem) {
  Selected cursor;
  if (!mem->read(MouseSystem::OUTPUT, cursor)) {
    return error("no cursor to display");
  }
  Camera camera;
  if (!mem->read("camera", camera)) {
    return error("no camera");
  }
  static auto cross = mem->createEntity("cursor");
  _renderer->draw(cross, Sphere3f(cursor.point, 0.1f), Mat4x4f::identity(),
                  camera.view(), camera.projection());
  return ok();
}

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow;
  Ptr<Keyboard> keyboard = window_impl->provideKeyboard();
  Ptr<Mouse> mouse = window_impl->provideMouse();
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
      .setMouse(mouse)
      .makeSystem<InitSystem>()
      .makeSystem<DebugHidSystem>(window, textRenderer)
      .makeSystem<FixedCameraSystem>(window)
      .makeSystem<HitBoxRenderingSystem>(shapeRenderer)
      .makeSystem<PhysicsSystem>(world)
      //.makeSystem<CollisionDetectionSystem>()
      //.makeSystem<CollisionRenderingSystem>(shapeRenderer)
      //.makeSystem<CollisionSolverSystem>()
      .makeSystem<MouseSystem>()
      .makeSystem<CursorRenderingSystem>(shapeRenderer);

  std::cout << "START: " << engine.start().message() << std::endl;
  std::cout << "RUN: " << engine.run().message() << std::endl;
  return 0;
}
