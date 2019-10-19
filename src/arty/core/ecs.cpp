#include <arty/core/ecs.h>

namespace arty {
uint64_t Entity::_next = 0;

Result System::process(Ptr<Blackboard> const &) { return ok(); }

Result System::init(Ptr<Blackboard> const &) { return ok(); }

void System::release() {}

void System::add_type(const ComponentStorage::type_t &type) {
  _types.push_back(type);
}

std::vector<ComponentStorage::type_t> System::find_missing_components(
    Ptr<Blackboard> const &board) {
  std::vector<ComponentStorage::type_t> result;
  return result;
}

}  // namespace arty
