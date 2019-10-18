#include <arty/core/ecs.h>

namespace arty {
uint64_t Entity::_next = 0;

ComponentStorage::Ptr Blackboard::get(ComponentStorage::type_t const &type) {
  for (auto storage : _components) {
    if (storage.second->type() == type) {
      return storage.second;
    }
  }
  return nullptr;
}

Result System::process(Ptr<Blackboard> const &) { return ok(); }

Result System::init(Ptr<Blackboard> const &) { return ok(); }

void System::release() {}

void System::add_type(const ComponentStorage::type_t &type) {
  _types.push_back(type);
}

std::vector<ComponentStorage::type_t> System::find_missing_components(
    Ptr<Blackboard> const &board) {
  std::vector<ComponentStorage::type_t> result;
  for (auto type : _types) {
    auto storage = board->get(type);
    if (!storage) {
      result.push_back(type);
    }
  }
  return result;
}

}  // namespace arty
