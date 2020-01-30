#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <any>
#include <arty/core/result.hpp>
#include <functional>
#include <map>
#include <string>

namespace arty {

/**
 * @brief The Blackboard class
 *
 * The blackoad represent the entire memory necessary for the current "stage"
 * of the game
 *
 * Entities are just ids, linking properties together
 * Entities represent piece of data necessary for the game
 * Thererefor they all have a position and a graphic representation
 *
 * Some of them are special and requires a privileged access, like cameras
 * or player characters (ie io related)
 *
 * This blackboad memory shall be highly dynamic
 * It should be possible to add properties without
 * the blackboad knowing what it is
 *
 * Following rule, if i don't want to save it, it does not belong here
 */

struct Entity {
 private:
  std::string _name;
  uint64_t _id;
  static uint64_t _count;

 public:
  Entity(std::string const& name, uint64_t id) : _name(name), _id(id) {}
  Entity() : _name(), _id(0) {}

  bool operator<(Entity const& rhs) const { return _id < rhs._id; }
  bool operator>(Entity const& rhs) const { return _id > rhs._id; }
  bool operator==(Entity const& rhs) const { return _id == rhs._id; }
  bool operator!=(Entity const& rhs) const { return !(*this == rhs); }
  bool operator>=(Entity const& rhs) const { return !(*this < rhs); }
  bool operator<=(Entity const& rhs) const { return !(*this > rhs); }
  bool isValid() const { return _id != 0 && !_name.empty(); }
  explicit operator bool() const { return isValid(); }

  static Entity generate(std::string const& name) {
    return Entity(name, ++_count);
  }

  std::string const& name() const { return _name; }
  uint64_t id() const { return _id; }
};

}  // namespace arty
namespace std {

template <>
class hash<arty::Entity> {
 public:
  size_t operator()(arty::Entity const& s) const {
    return std::hash<uint64_t>()(s.id());
  }
};
/*
static ostream& operator<<(ostream& out, arty::Entity const& e) {
  out << e.name() + "_" + std::to_string(e.id());
  return out;
}
*/
}  // namespace std

namespace arty {

class Memory {
 public:
  /**
   * @brief createEntity
   * @param name representing the entity
   * @return the real id created
   */
  Entity createEntity(std::string const& name) {
    return Entity::generate(name);
  }

  template <typename T>
  bool write(T const& val) {
    return write(Entity(), val);
  }

  template <typename T>
  bool write(Entity const& entity, T const& val) {
    _components[typeid(T).name()][entity] = val;
    return true;
  }

  template <typename T>
  bool read(Entity const& entity, T& val) {
    auto it = _components.find(typeid(T).name());
    if (it != _components.end()) {
      auto it2 = it->second.find(entity);
      if (it2 != it->second.end()) {
        try {
          val = std::any_cast<T>(it2->second);
          return true;
        } catch (const std::bad_any_cast&) {
          return false;
        }
      }
    }
    return false;
  }

  template <typename T>
  bool read(T& val) {
    return read(Entity(), val);
  }

  template <typename T>
  std::size_t count() const {
    auto it = _components.find(typeid(T).name());
    if (it != _components.end()) {
      return it->second.size();
    }
    return 0;
  }

  template <typename T>
  bool remove() {
    return _components.erase(typeid(T).name()) > 0;
  }

  template <typename T>
  bool remove(Entity const& entity) {
    return _components[typeid(T).name()].erase(entity) > 0;
  }

  bool remove(Entity const& entity) {
    for (auto& comp : _components) {
      comp.second.erase(entity);
    }
    return true;
  }

  void clear() { _components.clear(); }

  template <typename T>
  Result process(
      std::function<Result(Entity const& e, T const& comp)> updateFunc) {
    auto componentContainer = _components[typeid(T).name()];
    if (componentContainer.size() == 0) {
      return error("unknown component: " + typeid(T).name());
    }
    auto compIt = componentContainer.begin();
    auto itEnd = componentContainer.end();
    try {
      for (; compIt != itEnd; ++compIt) {
        T const* val = std::any_cast<T>(&compIt->second);
        Result r = updateFunc(compIt->first, *val);
        if (!r) {
          return r;
        }
      }
    } catch (const std::bad_any_cast& e) {
      return error(e.what());
    }

    return ok();
  }

  template <typename T1, typename T2>
  using ProcessFunc2 =
      std::function<Result(Entity const&, T1 const&, T2 const&)>;

  template <typename T1, typename T2>
  Result process(ProcessFunc2<T1, T2> updateFunc) {
    auto container1 = _components[typeid(T1).name()];
    if (container1.size() == 0) {
      return error("unknown component: " + typeid(T1).name());
    }
    auto c1It = container1.begin();
    auto c1ItEnd = container1.end();

    auto container2 = _components[typeid(T2).name()];
    if (container2.size() == 0) {
      return error("unknown component: " + typeid(T2).name());
    }
    auto c2It = container2.begin();
    auto c2ItEnd = container2.end();
    while (c1It != c1ItEnd && c2It != c2ItEnd) {
      auto e1 = c1It->first;
      auto e2 = c2It->first;
      if (e1 < e2) {
        ++c1It;
        continue;
      }
      if (e2 < e1) {
        ++c2It;
        continue;
      }
      T1 const* v1;
      T2 const* v2;
      try {
        v1 = std::any_cast<T1>(&c1It->second);
      } catch (const std::bad_any_cast& e) {
        return error(e.what());
      }
      try {
        v2 = std::any_cast<T2>(&c2It->second);
      } catch (const std::bad_any_cast& e) {
        return error(e.what());
      }
      if (!v1 || !v2) {
        return error("bad_any_cast");
      }
      check_result(updateFunc(c1It->first, *v1, *v2));
      ++c1It;
      ++c2It;
    }
    return ok();
  }

 private:
  std::map<std::string, std::map<Entity, std::any>> _components;
};

}  // namespace arty

#endif  // MEMORY_HPP
