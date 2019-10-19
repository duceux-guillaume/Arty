#include <arty/ext/opengl_renderer.h>
#include <arty/ext/opengl_window.h>
#include <arty/impl/engine.h>
#include <arty/impl/position_system.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Engine engine;
  engine.set_window(window)
      .add_system(Ptr<System>(new OpenGlRenderer))
      .add_system(Ptr<System>(new PositionSystem));

  Entity triangle("Triangle");
  Mesh mesh;
  mesh.vbo = 0;
  mesh.buffer = {
      -1.0f, -1.0f, -1.0f,  //
      -1.0f, -1.0f, 1.0f,   //
      -1.0f, 1.0f,  1.0f,   //
      1.0f,  1.0f,  -1.0f,  //
      -1.0f, -1.0f, -1.0f,  //
      -1.0f, 1.0f,  -1.0f,  //
      1.0f,  -1.0f, 1.0f,   //
      -1.0f, -1.0f, -1.0f,  //
      1.0f,  -1.0f, -1.0f,  //
      1.0f,  1.0f,  -1.0f,  //
      1.0f,  -1.0f, -1.0f,  //
      -1.0f, -1.0f, -1.0f,  //
      -1.0f, -1.0f, -1.0f,  //
      -1.0f, 1.0f,  1.0f,   //
      -1.0f, 1.0f,  -1.0f,  //
      1.0f,  -1.0f, 1.0f,   //
      -1.0f, -1.0f, 1.0f,   //
      -1.0f, -1.0f, -1.0f,  //
      -1.0f, 1.0f,  1.0f,   //
      -1.0f, -1.0f, 1.0f,   //
      1.0f,  -1.0f, 1.0f,   //
      1.0f,  1.0f,  1.0f,   //
      1.0f,  -1.0f, -1.0f,  //
      1.0f,  1.0f,  -1.0f,  //
      1.0f,  -1.0f, -1.0f,  //
      1.0f,  1.0f,  1.0f,   //
      1.0f,  -1.0f, 1.0f,   //
      1.0f,  1.0f,  1.0f,   //
      1.0f,  1.0f,  -1.0f,  //
      -1.0f, 1.0f,  -1.0f,  //
      1.0f,  1.0f,  1.0f,   //
      -1.0f, 1.0f,  -1.0f,  //
      -1.0f, 1.0f,  1.0f,   //
      1.0f,  1.0f,  1.0f,   //
      -1.0f, 1.0f,  1.0f,   //
      1.0f,  -1.0f, 1.0f    //

  };
  engine.set(triangle, "mesh", mesh);

  Entity triangle2("Triangle2");
  Mesh mesh2;
  mesh2.vbo = 0;
  mesh2.buffer = {
      -1.0f, -1.0f, 0.0f,  //
      0.0f,  0.0f,  0.0f,  //
      -1.0f, 1.0f,  0.0f,  //
  };
  engine.set(triangle2, "mesh", mesh2);

  auto res = engine.start();
  if (!res) {
    std::cerr << res.message() << std::endl;
    engine.stop();
    return res;
  }
  res = engine.run();
  if (!res) {
    std::cerr << res.message() << std::endl;
  }
  return res;
}
