#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/engine.hpp>
#include <arty/impl/mouse_system.hpp>
#include <arty/impl/physics_system.hpp>

#include "tile_systems.hpp"

using namespace arty;

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
  if (cursor.entity.id() > 0) {
    _renderer->draw(cross, Sphere3f(cursor.point, 0.2f), Mat4x4f::identity(),
                    camera.view(), camera.projection());
  } else {
    _renderer->draw(cross, Sphere3f(cursor.point, 0.1f), Mat4x4f::identity(),
                    camera.view(), camera.projection());
  }
  return ok();
}

int main(void) {
  GlfwWindow* window_impl = new GlfwWindow;
  Ptr<Keyboard> keyboard = window_impl->provideKeyboard();
  Ptr<Mouse> mouse = window_impl->provideMouse();
  Ptr<Window> window(window_impl);
  Ptr<Memory> board(new Memory);
  Ptr<IRenderer2D> textRenderer(new GlRenderer2D());
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
  cam_sys->setEye({0.f, 0.f, 15.f});
  cam_sys->setTarget({0.f, 0.f, 0.f});
  cam_sys->setUpdir({0.f, 0.f, 1.f});

  Engine engine;
  engine.setBoard(board)
      .setWindow(window)
      .setKeyboard(keyboard)
      .setMouse(mouse)
      .makeSystem<RandomBoardInitSystem>()
      .makeSystem<DebugHidSystem>(window, textRenderer)
      .addSystem(cam_sys)
      .makeSystem<TileRenderingSystem>(shapeRenderer)
      .makeSystem<HitBoxRenderingSystem>(shapeRenderer)
      .makeSystem<MouseSystem>()
      .makeSystem<CursorRenderingSystem>(shapeRenderer)
      .makeSystem<EventSystem>(leftClick, Event("SHOOT"), AddFunc);

  std::cout << "START: " << engine.start().message() << std::endl;
  std::cout << "RUN: " << engine.run().message() << std::endl;

  Vec2f test_{0.f, 5.f};
  return 0;
}
