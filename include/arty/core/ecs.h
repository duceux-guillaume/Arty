#ifndef ECS_H
#define ECS_H

#include <arty/core/result.h>

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace arty {

class Entity {
 public:
  using id_t = uint64_t;
  using Ptr = std::shared_ptr<Entity>;

  Entity(id_t id) : _id(id) {}

  static Entity create() {
    Entity ent(_next);
    _next++;
    return ent;
  }

  bool operator==(Entity const &e) const { return _id == e._id; }

  bool operator!=(Entity const &e) const { return !(*this == e); }

  bool operator<(Entity const &e) const { return _id < e._id; }

 private:
  id_t _id;
  static id_t _next;
};

class Component {};

class ComponentStorage {
 public:
  using type_t = std::string;
  using Ptr = std::shared_ptr<ComponentStorage>;

  type_t const &type() { return _type; }

  template <typename C>
  void set(Entity const &e, C const &val) {}

  template <typename C>
  Result get(Entity const &e, C *val) {
    return Result("nope");
  }

 private:
  type_t _type;
};

class Blackboard {
 public:
  using Ptr = std::shared_ptr<Blackboard>;

  void add(ComponentStorage::Ptr const &ptr);
  ComponentStorage::Ptr get(ComponentStorage::type_t const &type);

  bool exist(Entity const &id) const {
    return _entities.find(id) != _entities.end();
  }

  void create(Entity const &id) { _entities.insert(id); }

 private:
  std::set<Entity> _entities;
  std::unordered_map<ComponentStorage::type_t, ComponentStorage::Ptr>
      _components;
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

template <typename C>
class DenseComponentStorage : public ComponentStorage {
 public:
  DenseComponentStorage(ComponentStorage::type_t const &type) : _type(type) {}

  void set(Entity const &e, C const &val) { _data[e] = val; }

 private:
  ComponentStorage::type_t _type;
  std::vector<C> _data;
};

};  // namespace arty

#endif  // ECS_H
