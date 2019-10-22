#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <arty/core/entity.h>
#include <arty/core/property.h>
#include <arty/core/result.h>

#include <cassert>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace arty {

template <typename T>
using Storage = std::vector<Property<T>>;
template <typename T>
using Holder = Ptr<Storage<T>>;

class Blackboard {
 public:
  template <typename T>
  Holder<T> getProperty(std::string const& property_name) {
    return items<T>[property_name];
  }

  template <typename T>
  void set(Entity const& ent, std::string const& prop, const T& t) {
    if (!items<T>[prop]) {
      items<T>[prop] = std::make_shared<Storage<T>>();
    }
    // new entity
    if (entities.count(ent.id()) == 0) {
      auto pos = push(ent, prop, t);
      entities[ent.id()][prop] = pos;
      return;
    }
    // new property
    if (entities[ent.id()].count(prop) == 0) {
      auto pos = push(ent, prop, t);
      entities[ent.id()][prop] = pos;
      return;
    }
    // existing prop
    auto pos = entities[ent.id()][prop];
    auto prop_ptr = items<T>[prop];
    assert(prop_ptr);  // we already verified that in theory
    if ((*prop_ptr)[pos].entity() == ent) {
      (*prop_ptr)[pos] = Property<T>(ent, t);
      return;
    }
    // omg the position changed
    std::cout << "TODO find the new position of the property" << std::endl;
  }

  template <typename T>
  T* getEntityProperty(Entity const& ent, std::string const& property_name) {
    // TODO find a faster way to avoid doing that
    auto ptr = items<T>[property_name];
    if (!ptr) {
      return nullptr;
    }
    for (auto& prop : *ptr) {
      if (prop.entity() == ent) {
        return &prop.val();
      }
    }
    return nullptr;
  }

 private:
  template <class T>
  static std::unordered_map<std::string, Holder<T>> items;
  std::unordered_map<uint64_t, std::unordered_map<std::string, size_t>>
      entities;

  template <typename T>
  size_t push(Entity const& entity, std::string const& prop, const T& t) {
    auto ptr = items<T>[prop];
    assert(ptr);  // should be verified before calling this method
    ptr->push_back(Property<T>(entity, t));
    return ptr->size() - 1;
  }
};

template <class T>
std::unordered_map<std::string, Holder<T>> Blackboard::items;

}  // namespace arty

#endif  // BLACKBOARD_H
