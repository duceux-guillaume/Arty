#include <arty/impl/player_control.hpp>

namespace arty {

Result PlayerControlSystem::process(const Ptr<Memory> & /*board*/,
                                    const Ptr<Keyboard> &keyboard) {
  if (keyboard->hasOccured(_up)) {
    std::cout << "move up" << std::endl;
  }

  return ok();
}

Result PlayerControlSystem::init(const Ptr<Memory> &, const Ptr<Keyboard> &) {
  /*
  auto playersPtr = board->getProperties<Entity>("players");
  if (!playersPtr) {
    return error("no players");
  }
  if (playersPtr->size() == 0) {
    return error("no players");
  }
  auto player1 = playersPtr->at(0);

  _up = InputEvent("move_up");
  if (!keyboard->registerKeyEvent(Keyboard::W, Keyboard::Action::PRESS, _up)) {
    return error("could'nt register event");
  }
  */
  return ok();
}

void PlayerControlSystem::release() {}

}  // namespace arty
