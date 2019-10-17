#include <arty/ext/OpenGlWindow.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>

template <typename T>
using Ptr = std::shared_ptr<T>;

using namespace arty;

int main(void) {
  Ptr<Window> window(new OpenGlWindow);

  auto res = window->init();
  if (!res) {
    std::cerr << res.message() << std::endl;
    return 1;
  }

  do {
    window->clear();
    // Draw nothing, see you in tutorial 2 !

    window->swapBuffer();

  }  // Check if the ESC key was pressed or the window was closed
  while (window->isOk());

  window->close();
  return 0;
}
