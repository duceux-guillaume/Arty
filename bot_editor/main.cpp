#include <arty/ext/opengl/opengl_renderer.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
#include <arty/ext/glfw/glfw_window.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/ext/opengl/gl_shape_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_rendering_system.hpp>
#include <arty/impl/collision_solver_system.hpp>
#include <arty/impl/collision_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/mesh_loader_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace arty;

class BotFactory {
 public:
  BotFactory(Ptr<Blackboard> board) : board(board) {}

  void createCore() {
    auto core = board->createEntity("core");
    Mesh coreMesh;
    loader.loadObj("../models/Core.obj", &coreMesh);
    coreBox = collision.computeAxisAlignedBoundingBox(coreMesh);
    corePhy = Physics(1.f);

    board->set(core, "model2load", std::string("../models/Core.obj"));
    board->set(core, "transform", Transform(Vec3f{0.f, 0.f, 0.f}));
    board->set(core, "physics", Physics());
    board->set(core, "transform", Transform());
  }

  void addToSlot(std::string const& name, std::string const& model,
                 Vec3f const& dir) {
    auto module = board->createEntity(name);
    Mesh moduleMesh;
    loader.loadObj(model, &moduleMesh);
    Box moduleBox = collision.computeAxisAlignedBoundingBox(moduleMesh);
    corePhy.childs.push_back(module);

    Transform moduleOffset;
    moduleOffset.translation =
        dir.apply_with(coreBox.halfLength + moduleBox.halfLength,
                       [](float f1, float f2) { return f1 * f2; }) -
        moduleBox.center + coreBox.center;

    board->set(module, "model2load", model);
    board->set(module, "transform", Transform(Vec3f{0.f, 0.f, 0.f}));
    board->set(module, "physics", Physics());
    board->set(module, "transform", moduleOffset);
  }

 private:
  Entity core;
  Physics corePhy;
  Box coreBox;
  Ptr<Blackboard> board;
  CollisionDetection collision;
  Loader loader;
};

int main(void) {
  Ptr<Window> window(new GlfwWindow);
  Ptr<Blackboard> board(new Blackboard);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());
  Ptr<IShapeRenderer> shapeRenderer(new GlShapeRenderer());
  WorldPhysics world;
  world.gravity_strengh = 0.0f;
  world.air_friction = 1.f;
  world.ground_friction = 0.9f;
  Engine engine;
  engine.set_board(board)
      .set_window(window)
      .add_system(Ptr<System>(new PhysicsSystem(world)))
      .add_system(Ptr<System>(new CollisionSolverSystem()))
      .add_system(Ptr<System>(new FixedCameraSystem(window)))
      .add_system(Ptr<System>(new CollisionRenderingSystem(shapeRenderer)))
      .add_system(Ptr<System>(new DebugHidSystem(window, textRenderer)))
      .add_system(Ptr<System>(new MeshLoaderSystem))
      .add_system(Ptr<System>(new OpenGlRenderer))
      .add_system(Ptr<System>(new CollisionSystem()));

  BotFactory factory(board);
  factory.createCore();
  factory.addToSlot("energy", "../models/energy.obj", Vec3f(0.f, 0.9f, 0.f));
  factory.addToSlot("radar", "../models/radar.obj", Vec3f(0.f, 0.f, 1.f));
  factory.addToSlot("shield", "../models/shield.obj", Vec3f(0.f, -0.5f, 0.25f));
  factory.addToSlot("tracks", "../models/tracks.obj", Vec3f(0.f, 0.f, -1.f));
  factory.addToSlot("rocket", "../models/rocket.obj", Vec3f(-1.f, 0.f, 0.f));
  factory.addToSlot("railgun", "../models/railgun.obj", Vec3f(1.f, -0.1f, 0.f));
  factory.addToSlot("wheels", "../models/wheels.obj", Vec3f(0.f, 0.f, -1.f));

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
