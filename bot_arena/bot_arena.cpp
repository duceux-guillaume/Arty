#include <arty/ext/opengl_renderer.h>
#include <arty/ext/opengl_window.h>
#include <arty/impl/engine.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Engine engine;
  engine.set_window(window).add_system(Ptr<System>(new OpenGlRenderer));
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
