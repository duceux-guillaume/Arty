#ifndef POSITION_SYSTEM_H
#define POSITION_SYSTEM_H

#include <arty/core/ecs.h>
#include <arty/core/window.h>

namespace arty {

struct Position {
  float position[3];
  float orientation[4];
};

}  // namespace arty

std::ostream& operator<<(std::ostream& os, arty::Position const& r);

namespace arty {

class PositionComponent : public DenseComponentStorage<Position> {
 public:
  PositionComponent(ComponentStorage::type_t const& type)
      : DenseComponentStorage<Position>(type) {}

 private:
};

class PositionSystem : public System {
 public:
  PositionSystem(Ptr<Window> const& w) : System(), _window(w) {}

  Result init(Ptr<Blackboard> const& board) override { return ok(); }

  Result process(Ptr<Blackboard> const& board) override {
    // Get the position we want to move
    Position p;
    auto pentity = board->getProperty<Entity>("keyboard");
    {
      if (!pentity) {
        return error("no entity being controlled");
      }
      if (pentity->size() > 1) {
        return error("multiple control entities is not implemented");
      }
      auto pposition = board->getEntityProperty<Position>(
          pentity->front().val(), "position");
      if (!pposition) {
        return error("entity has no position property");
      }
      p = *pposition;
    }

    static double lastTime = _window->getTime();
    double currentTime = _window->getTime();
    double deltaTime = currentTime - lastTime;
    CursorPosition cursor = _window->getCursorPosition();
    _window->setCursorPosition(
        CursorPosition(_window->width() / 2.0, _window->height() / 2.0));

    if (_window->keyHasBeenPressed(Key::UP)) {
      p.position[0] += 1.f;
    }
    board->set(pentity->front().val(), "position", p);
    std::cout << "set: " << pentity->front().val().name() << " position "
              << p.position[0] << std::endl;
    return ok();
  }

 private:
  Ptr<Window> _window;
};

}  // namespace arty

#endif  // POSITION_SYSTEM_H
