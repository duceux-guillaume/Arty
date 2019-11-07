#include <arty/ext/opengl/opengl_renderer.h>
#include <arty/ext/opengl/opengl_window.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
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

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
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
      .add_system(Ptr<System>(new CollisionSolverSystem()))
      .add_system(Ptr<System>(new PhysicsSystem(world)))
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new CollisionRenderingSystem(shapeRenderer)))
      .add_system(Ptr<System>(new DebugHidSystem(window, textRenderer)))
      .add_system(Ptr<System>(new MeshLoaderSystem))
      .add_system(Ptr<System>(new OpenGlRenderer))
      .add_system(Ptr<System>(new CollisionSystem()));

  auto core = board->createEntity("core");
  board->set(core, "model2load", std::string("../models/Core.obj"));
  board->set(core, "transform", Transform(Vec3f{0.f, 0.f, 0.f}));
  board->set(core, "physics", Physics());

  auto energy = board->createEntity("energy");
  board->set(energy, "model2load", std::string("../models/energy.obj"));
  board->set(energy, "transform", Transform(Vec3f{0.f, 0.f, 0.f}));
  board->set(energy, "physics", Physics());

  auto radar = board->createEntity("radar");
  board->set(radar, "model2load", std::string("../models/radar.obj"));
  board->set(radar, "transform", Transform(Vec3f{0.f, 0.f, 2.f}));
  board->set(radar, "physics", Physics());

  auto railgun = board->createEntity("railgun");
  board->set(railgun, "model2load", std::string("../models/railgun.obj"));
  board->set(railgun, "transform", Transform(Vec3f{0.f, 1.f, 1.f}));
  board->set(railgun, "physics", Physics());

  auto rocket = board->createEntity("rocket");
  board->set(rocket, "model2load", std::string("../models/rocket.obj"));
  board->set(rocket, "transform", Transform(Vec3f{0.f, 1.f, 1.f}));
  board->set(rocket, "physics", Physics());

  auto shield = board->createEntity("shield");
  board->set(shield, "model2load", std::string("../models/shield.obj"));
  board->set(shield, "transform", Transform(Vec3f{0.f, -1.f, 1.f}));
  board->set(shield, "physics", Physics());

  auto track = board->createEntity("track");
  board->set(track, "model2load", std::string("../models/track.obj"));
  board->set(track, "transform", Transform(Vec3f{0.f, 0.f, 0.f}));
  board->set(track, "physics", Physics());

  auto wheel = board->createEntity("wheel");
  board->set(wheel, "model2load", std::string("../models/wheels.obj"));
  board->set(wheel, "transform", Transform(Vec3f{0.f, 0.f, 0.f}));
  board->set(wheel, "physics", Physics());

  auto cube = board->createEntity("cube");
  board->set(cube, "model2load", std::string("../models/test_cube.obj"));
  Mat4x4f tf = /*rotation(1.f, 0.f, 0.f) +*/ translation(0.f, 0.f, 20.f);
  board->set(cube, "transform", Transform::from(tf));
  board->set(cube, "camtarget", cube);
  board->set(cube, "physics", Physics());

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
