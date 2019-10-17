#include <arty/ext/OpenGlWindow.h>
#include <arty/impl/engine.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);
  Engine engine(window);
  auto res = engine.start();
  if (!res) {
    std::cerr << res.message() << std::endl;
    return 1;
  }
  res = engine.run();
  engine.stop();
  return res;
}
