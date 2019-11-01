#include <arty/ext/opengl/opengl_renderer.h>
#include <arty/ext/opengl/opengl_window.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
#include <arty/ext/opengl/2d_renderer.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/debug_hid_system.hpp>
#include <arty/impl/mesh_loader_system.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Ptr<Blackboard> board(new Blackboard);
  Ptr<ITextRenderer> textRenderer(new GlTextRenderer());
  Engine engine;
  engine.set_board(board)
      .set_window(window)
      .add_system(Ptr<System>(new DebugHidSystem(window, textRenderer)))
      .add_system(Ptr<System>(new MeshLoaderSystem))
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new OpenGlRenderer));

  auto pyramid = board->createEntity("pyramid");
  board->set(pyramid, "model2load", std::string("../models/test_pyramid.obj"));
  board->set(pyramid, "transform", Transform(Vec3f{3.f, 0.f, 0.f}));

  auto arena = board->createEntity("arena");
  board->set(arena, "model2load", std::string("../models/bot.obj"));
  board->set(arena, "transform", Transform());

  auto cube = board->createEntity("cube");
  board->set(cube, "model2load", std::string("../models/test_cube.obj"));
  board->set(cube, "transform", Transform(Vec3f{-3.f, 0.f, 0.f}));
  board->set(cube, "camtarget", cube);

  check_result(engine.start());
  check_result(engine.run());
  return ok();
}
