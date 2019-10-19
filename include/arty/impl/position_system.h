#ifndef POSITION_SYSTEM_H
#define POSITION_SYSTEM_H

#include <arty/core/ecs.h>

namespace arty {

struct Position {
  float pos[3];
  float rot[4];
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
  PositionSystem() : System() { System::_types.push_back("Position"); }

  Result init(Ptr<Blackboard> const& board) override { return ok(); }

  Result process(Ptr<Blackboard> const& board) override {
    auto component_iterator = board->get_property<Position>("position");
    for (auto& pos : *component_iterator) {
      // std::cout << pos << std::endl;
    }
    return ok();
  }

 private:
};

}  // namespace arty

#endif  // POSITION_SYSTEM_H
