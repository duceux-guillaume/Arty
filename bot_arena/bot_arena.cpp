#include <arty/ext/opengl_renderer.h>
#include <arty/ext/opengl_window.h>
#include <arty/impl/engine.h>

#include <arty/core/mesh.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/mesh_loader_system.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  /*
  Ptr<Window> window(new OpenGlWindow);
  Engine engine;
  engine.set_window(window)
      .add_system(Ptr<System>(new CameraSystem(window)))
      .add_system(Ptr<System>(new OpenGlRenderer));

  Factory factory;
  auto res = factory.name("test")
                 .position(0.f, 0.f, 0.f)
                 .shaders("../shaders/StandardShading.vertexshader",
                          "../shaders/StandardShading.fragmentshader")
                 .obj("../models/uv_test/tower.obj")
                 .build(engine.board());
  check_result(res);

  res = engine.start();
  if (!res) {
    std::cerr << res.message() << std::endl;
    engine.stop();
    return res;
  }

  res = engine.run();
  check_result(res);
  */
  Loader loader;
  Mesh mymesh;
  // loader.load("../models/obstacles.obj", &mymesh);
  // loader.load("../models/arena_test.obj", &mymesh);
  // loader.load("../models/bot.obj", &mymesh);
  loader.load("../models/uv_test/tower.obj", &mymesh);

  Ptr<Window> window(new OpenGlWindow);
  window->init();
  Renderer renderer;
  check_result(renderer.init());
  renderer.importStaticMeshNoTexture(mymesh);

  size_t count = 0;
  Mat4x4f proj = perspective(radians(45.f), 4.0f / 3.0f, 0.1f, 100.0f);
  Mat4x4f view = translation(0.f, 0.f, -10.f);
  Mat4x4f model = rotation(0.f, count / 10000.f * 3.14f, 0.f);

  // For speed computation
  double lastTime = window->getTime();
  int nbFrames = 0;
  do {
    window->clear();
    // Measure speed
    double currentTime = window->getTime();
    nbFrames++;
    // If last prinf() was more than 1sec ago
    if (currentTime - lastTime >= 1.0) {
      // printf and reset
      std::cout << (currentTime - lastTime) * 1000. / double(nbFrames)
                << " ms/frame" << std::endl;
      nbFrames = 0;
      lastTime = currentTime;
    }
    model = rotation(0.f, count / 300.f * 3.14f, 0.f);
    renderer.drawStaticMeshNoTexture(mymesh, model, view, proj);
    window->swapBuffer();
    count++;
  } while (window->isOk() && count < 10000);
  renderer.release();
  return ok();
}
