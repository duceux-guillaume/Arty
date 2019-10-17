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

}  // namespace arty

std::ostream &operator<<(std::ostream &os, const arty::Result &r) {
  os << r.message();
  return os;
}
