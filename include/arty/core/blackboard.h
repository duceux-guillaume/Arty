#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <arty/core/entity.h>
#include <arty/core/math.h>
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
template <typename T>
using PropMap = std::unordered_map<std::string, Holder<T>>;

class Blackboard {
 public:
  Holder<Mat4x4f> getProperty(std::string const& property_name) {
    return mats[property_name];
  }

  void set(Entity const& ent, std::string const& prop, const Mat4x4f& t) {
    if (!mats[prop]) {
      mats[prop] = std::make_shared<Storage<Mat4x4f>>();
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
    auto prop_ptr = mats[prop];
    assert(prop_ptr);  // we already verified that in theory
    if ((*prop_ptr)[pos].entity() == ent) {
      (*prop_ptr)[pos] = Property<Mat4x4f>(ent, t);
      return;
    }
    // omg the position changed
    std::cout << "TODO find the new position of the property" << std::endl;
  }

  template <typename T>
  T* getEntityProperty(Entity const& ent, std::string const& property_name) {
    // TODO find a faster way to avoid doing that
    auto ptr = mats[property_name];
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
  PropMap<Mat4x4f> mats;
  std::unordered_map<uint64_t, std::unordered_map<std::string, size_t>>
      entities;

  template <typename T>
  size_t push(Entity const& entity, std::string const& prop, const T& t) {
    auto ptr = mats[prop];
    assert(ptr);  // should be verified before calling this method
    ptr->push_back(Property<T>(entity, t));
    return ptr->size() - 1;
  }
};

}  // namespace arty

#endif  // BLACKBOARD_H
