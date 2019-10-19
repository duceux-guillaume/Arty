#ifndef ECS_H
#define ECS_H

#include <arty/core/blackboard.h>
#include <arty/core/entity.h>
#include <arty/core/result.h>

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace arty {

class Component {
 public:
  Entity id;

 private:
};

class ComponentStorage {
 public:
  using type_t = std::string;

  ComponentStorage(type_t const &t) : _type(t) {}

  type_t const &type() { return _type; }

  template <typename C>
  void set(Entity const &e, C const &val) {}

  template <typename C>
  Result get(Entity const &e, C *val) {
    return Result("nope");
  }

 protected:
  type_t _type;
};

template <typename C>
class DenseComponentStorage : public ComponentStorage {
 public:
  DenseComponentStorage(ComponentStorage::type_t const &type)
      : ComponentStorage(type) {}

  void set(Entity const &e, C const &val) { _data[e] = val; }

 protected:
  std::vector<C> _data;
};

class System {
 public:
  virtual Result process(Ptr<Blackboard> const &board);
  virtual Result init(Ptr<Blackboard> const &board);
  virtual void release();

  void add_type(ComponentStorage::type_t const &type);
  std::vector<ComponentStorage::type_t> find_missing_components(
      Ptr<Blackboard> const &board);

 protected:
  std::vector<ComponentStorage::type_t> _types;
};

};  // namespace arty

#endif  // ECS_H
