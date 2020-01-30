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

void makeCube(std::string const& name, Tf3f const& pos, Vec3f const& length,
              float mass, Ptr<Memory> mem) {
  auto entity = mem->createEntity(name);
  mem->write(entity, AABox3f(Vec3f(0.f, 0.f, 0.f), length));
  mem->write(entity, Physics(pos, mass));
}

class InitSystem : public System {
 private:
  Event _reset;

  void reset(Ptr<Memory> const& mem) {
    mem->clear();
    makeCube("floor", Tf3f(), Vec3f(5.f, 5.f, 1.f), 0.f, mem);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> xdis(-5.f, 5.f);
    std::uniform_real_distribution<> ydis(-5.f, 5.f);
    std::uniform_real_distribution<> zdis(5.f, 20.f);
    std::uniform_real_distribution<> lendis(0.1f, 2.f);
    for (int n = 0; n < 50; ++n) {
      makeCube("random", Tf3f(Vec3f(xdis(gen), ydis(gen), zdis(gen))),
               Vec3f(lendis(gen), lendis(gen), lendis(gen)), 1.f, mem);
    }
  }

 public:
  InitSystem() : _reset("RESET") {}

  Result process(const Ptr<Memory>& mem,
                 Ptr<InputManager> const& input) override {
    if (input->pop(_reset)) {
      reset(mem);
    }
    return ok();
  }
  Result init(const Ptr<Memory>& mem, Ptr<InputManager> const& input) override {
    if (!input->attach(Keyboard::SPACE, Keyboard::Action::PRESS, _reset)) {
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
  if (!mem->read(cursor)) {
    return error("no cursor to display");
  }
  Camera camera;
  if (!mem->read(camera)) {
    return error("no camera");
  }
  static auto cross = mem->createEntity("cursor");
  _renderer->draw(cross, AABox3f(cursor.point, Vec3f::all(1.f)),
                  Mat4x4f::identity(), camera.view(), camera.projection());
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

  auto AddFunc = [](Ptr<Memory> const& mem) -> Result {
    Selected cursor;
    if (!mem->read(cursor)) {
      return error("no cursor to display");
    }
    makeCube("s", cursor.point, Vec3f::all(1.f), 1.f, mem);
    return ok();
  };

  auto RmFunc = [](Ptr<Memory> const& mem) -> Result {
    Selected cursor;
    if (!mem->read(cursor)) {
      return error("no 3d cursor");
    }
    if (cursor.entity) {
      mem->remove(cursor.entity);
    }
    return true;
  };

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
      .makeSystem<CollisionDetectionSystem>()
      .makeSystem<CollisionRenderingSystem>(shapeRenderer)
      .makeSystem<CollisionSolverSystem>()
      .makeSystem<MouseSystem>()
      .makeSystem<CursorRenderingSystem>(shapeRenderer)
      .makeSystem<EventSystem>(
          Input(Mouse::Button::LEFT, Device::Action::PRESS), Event("SPAWN"),
          AddFunc)
      .makeSystem<EventSystem>(
          Input(Mouse::Button::RIGHT, Device::Action::PRESS), Event("DELETE"),
          RmFunc);

  auto start = engine.start();
  std::cout << "START: " << start.message() << std::endl;
  if (!start) {
    return -1;
  }
  std::cout << "RUN: " << engine.run().message() << std::endl;
  return 0;
}
