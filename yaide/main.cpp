#include <arty/ext/sfml/sfml_window.hpp>
#include <arty/impl/engine.hpp>

using namespace arty;

int main(void) {
  SfmlWindow* window_impl = new SfmlWindow;
  Ptr<Window> window(window_impl);
  Ptr<Memory> board(new Memory);

  Engine engine;
  engine.setBoard(board).setWindow(window);

  std::cout << "START: " << engine.start().message() << std::endl;
  std::cout << "RUN: " << engine.run().message() << std::endl;
  return 0;
}