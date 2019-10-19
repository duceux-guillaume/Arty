#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <arty/core/entity.h>
#include <arty/core/property.h>
#include <arty/core/result.h>

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
  Holder<T> get_property(std::string const& property_name) {
    std::cout << "get: " << property_name << " "
              << items<T>[property_name] -> size() << std::endl;
    return items<T>[property_name];
  }

  template <typename T>
  void set(Entity const& entity, std::string const& property_name, const T& t) {
    if (!items<T>[property_name]) {
      items<T>[property_name] = std::make_shared<Storage<T>>();
    }
    items<T>[property_name] -> push_back(Property<T>(entity, t));
    std::cout << "add: " << property_name << " "
              << items<T>[property_name] -> size() << std::endl;
  }

 private:
  template <class T>
  static std::unordered_map<std::string, Holder<T>> items;
};

template <class T>
std::unordered_map<std::string, Holder<T>> Blackboard::items;

}  // namespace arty

#endif  // BLACKBOARD_H
