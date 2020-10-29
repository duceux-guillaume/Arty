#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/mouse_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <random>

using namespace arty;

class InitSystem : public System {
 private:
  Event _reset;

  void reset(Ptr<Memory> const& mem) {
    mem->clear();
    Vec3f length(0.5f, 0.5f, 0.5f);
    for (uint8_t i = 0; i < 10; ++i) {
      for (uint8_t j = 0; j < 5; ++j) {
        auto entity = mem->createEntity("tile");
        mem->write(entity, Vec2ui8(i, j));
      }
    }
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

class TileRenderingSystem : public System {
 public:
  TileRenderingSystem(Ptr<IShapeRenderer> rend) : _renderer(rend) {}

 private:
  Ptr<IShapeRenderer> _renderer;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override {
    Camera cam;
    if (!board->read<Camera>(cam)) {
      return error("no camera");
    }

    if (board->count<Vec2ui8>()) {  // AABB
      auto work = [=](Entity const& e, Vec2ui8 const& pos) -> Result {
        float x = pos.x() - 5.f;
        float y = pos.y() - 2.5f;
        AABox3f box(Vec3f(x, y, 0.f), Vec3f(0.4f, 0.4f, 0.1f));
        Mat4x4f tf = Mat4x4f::identity();
        tf.setCol(2, {x, y, 0, 1});
        _renderer->draw(e, box, tf, cam.view(), cam.projection());
        return ok();
      };
      board->process<Vec2ui8>(work);
    }

    return ok();
  }
  Result init(const Ptr<Memory>& /*board*/) override {
    return_if_error(_renderer->init());
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

  auto AddFunc = [](Ptr<Memory> const& mem) -> Result {
    Selected cursor;
    if (!mem->read(cursor)) {
      return error("no cursor to display");
    }
    // Let's shoot
    auto bullet = mem->createEntity("bullet");
    mem->write(bullet, AABox3f(Vec3f(0.f, 0.f, 0.f), Vec3f::all(0.1f)));
    Particle p;
    p.position = vector_t(-10, 0, 2);
    p.setMass(0.1);
    number_t strength(100);
    vector_t tdc(cursor.point);
    vector_t dir = (tdc - p.position).normalize();
    p.velocity = dir * strength;
    mem->write(bullet, p);
    return ok();
  };

  Input leftClick(Mouse::Button::LEFT, Device::Action::PRESS);

  // Camera
  Ptr<FixedCameraSystem> cam_sys(new FixedCameraSystem(window));
  cam_sys->setEye({0.f, 0.f, 20.f});
  cam_sys->setTarget({0.f, 0.f, 0.f});
  cam_sys->setUpdir({0.f, 0.f, 1.f});

  Engine engine;
  engine.setBoard(board)
      .setWindow(window)
      .setKeyboard(keyboard)
      .setMouse(mouse)
      .makeSystem<InitSystem>()
      .makeSystem<DebugHidSystem>(window, textRenderer)
      .addSystem(cam_sys)
      .makeSystem<TileRenderingSystem>(shapeRenderer)
      //.makeSystem<PhysicsSystem>()
      //.makeSystem<CollisionRenderingSystem>(shapeRenderer)
      //.makeSystem<CollisionSolverSystem>()
      //.makeSystem<MouseSystem>()
      //.makeSystem<CursorRenderingSystem>(shapeRenderer)
      .makeSystem<EventSystem>(leftClick, Event("SHOOT"), AddFunc);

  std::cout << "START: " << engine.start().message() << std::endl;
  std::cout << "RUN: " << engine.run().message() << std::endl;
  return 0;
}
