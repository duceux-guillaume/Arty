#ifndef BLACKBOARD_HPP
#define BLACKBOARD_HPP

#include <arty/core/math.h>
#include <arty/core/result.h>

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

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
  std::string name;
  uint64_t id;

  Entity(std::string const& _name, uint64_t _id) : name(_name), id(_id) {}
  Entity() : name(), id(0) {}

  bool operator<(Entity const& rhs) const { return id < rhs.id; }
  bool operator>(Entity const& rhs) const { return id > rhs.id; }
  bool operator==(Entity const& rhs) const { return id == rhs.id; }
  bool operator!=(Entity const& rhs) const { return !(id == rhs.id); }
  bool isValid() const { return id != 0 && !name.empty(); }
};

}  // namespace arty
namespace std {

template <>
class hash<arty::Entity> {
 public:
  size_t operator()(arty::Entity const& s) const {
    return std::hash<uint64_t>()(s.id);
  }
};

}  // namespace std

namespace arty {

template <typename T>
struct Property {
  Entity entity;
  T value;

  Property(Entity const& _entity, T const& _val)
      : entity(_entity), value(_val) {}
  Property() : entity(), value() {}
};

class IStorage {
 public:
  virtual void clear() = 0;
  virtual std::size_t size() const = 0;
  virtual bool empty() const = 0;
  virtual bool remove(Entity const& entity) = 0;

 protected:
};

template <typename T>
class Iterator {
 public:
  using value_type = Property<T>;
  using reference = Property<T>&;
  using pointer = Property<T>*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  Iterator(typename std::vector<Property<T>>::iterator begin,
           typename std::vector<Property<T>>::iterator end)
      : _vector_curr(begin), _vector_end(end) {
    if (begin == end) {
      return;
    }
    if (!_vector_curr->entity.isValid()) {
      next();
    }
  }

  reference operator*() { return *_vector_curr; }
  // pointer operator& () { return &**this ; }
  pointer operator->() { return &**this; }  // *** EDIT

  void next() {
    if (_vector_curr == _vector_end) {
      return;
    }
    do {
      ++_vector_curr;
    } while (_vector_curr != _vector_end && !_vector_curr->entity.isValid());
  }

  Iterator& operator++() {
    next();
    return *this;
  }

  Iterator operator++(int) {
    const auto temp(*this);
    ++*this;
    return temp;
  }

  bool operator==(const Iterator& that) const {
    return _vector_curr == that._vector_curr;
  }
  bool operator!=(const Iterator& that) const { return !(*this == that); }

 private:
  typename std::vector<Property<T>>::iterator _vector_curr;
  typename std::vector<Property<T>>::iterator _vector_end;
};

template <typename T>
class RealStorage : public IStorage {
 public:
  using value_type = T;

  RealStorage() : _buffer(), _count(0) {}

  void set(Entity const& entity, T const& val) {
    T* ptr = get(entity);
    if (!ptr) {
      ++_count;
      _buffer.resize(entity.id);
      _buffer[entity.id - 1] = Property<T>(entity, val);
    } else {
      *ptr = val;
    }
  }

  Iterator<T> begin() { return Iterator<T>(_buffer.begin(), _buffer.end()); }
  Iterator<T> end() { return Iterator<T>(_buffer.end(), _buffer.end()); }

  T* get(Entity const& entity) {
    if (entity.id - 1 >= _buffer.size()) {
      return nullptr;
    }
    Property<T>& e = _buffer[entity.id - 1];
    if (!e.entity.isValid()) {
      return nullptr;
    }
    return &e.value;
  }

  Property<T> at(std::size_t pos) {
    std::size_t i = 0;
    for (auto const& prop : _buffer) {
      if (!prop.entity.isValid()) {
        continue;
      }
      if (i == pos) {
        return prop;
      }
      ++i;
    }
    return Property<T>();
  }

  void clear() override {
    _buffer.clear();
    _count = 0;
  }

  std::size_t size() const override { return _count; }
  bool empty() const override { return _count == 0; }

  bool remove(Entity const& entity) override {
    if (entity.id - 1 >= _buffer.size()) {
      return false;
    }
    Entity& e = _buffer[entity.id - 1].entity;
    if (!e.isValid()) {
      return false;
    }
    // Set entity to invalid, leave data alone this is enough
    _buffer[entity.id - 1].entity = Entity();
    _count--;
    return true;
  }

 private:
  std::vector<Property<T>> _buffer;
  std::size_t _count;
};

using BaseStorage = Ptr<IStorage>;
template <typename T>
using Container = Ptr<RealStorage<T>>;

class BlackboardImpl {
 public:
  BlackboardImpl() : _properties(), _entities(), _uuid(0) {}

  /**
   * @brief createEntity
   * @param name representing the entity
   * @return the real id created
   */
  Entity createEntity(std::string const& name) {
    ++_uuid;
    std::string newname = name + std::string("_") + std::to_string(_uuid);
    Entity candidate(newname, _uuid);
    _entities.insert(candidate);
    return candidate;
  }

  /**
   * @brief set value to property of entity
   */
  template <typename T>
  bool set(Entity const& entity, std::string const& property, T const& val) {
    if (_properties.count(property) == 0) {
      _properties[property] = BaseStorage(new RealStorage<T>());
    }
    auto ptr = pointer_cast<T>(_properties[property]);
    if (ptr) {
      ptr->set(entity, val);
      return true;
    }
    return false;
  }

  template <typename T>
  T* getEntityProperty(Entity const& entity, std::string const& property) {
    auto ptr = getProperties<T>(property);
    if (!ptr) {
      return nullptr;
    }
    return ptr->get(entity);
  }

  template <typename T>
  Container<T> getProperties(std::string const& property) {
    if (_properties.count(property) == 0) {
      return Container<T>(nullptr);
    }
    if (_properties[property]->size() == 0) {
      return Container<T>(nullptr);
    }
    return pointer_cast<T>(_properties[property]);
  }

  bool clearProperties(std::string const& property) {
    if (_properties.count(property)) {
      _properties[property]->clear();
      return true;
    }
    return false;
  }

  bool remove(Entity const& entity, std::string const& property) {
    if (_properties.count(property) > 0) {
      return _properties[property]->remove(entity);
    }
    return false;
  }

 private:
  std::map<std::string, Ptr<IStorage>> _properties;
  std::set<Entity> _entities;
  std::size_t _uuid;

  template <typename T>
  Container<T> pointer_cast(BaseStorage r) {
    auto p = static_cast<typename Container<T>::element_type*>(r.get());
    return Container<T>(r, p);
  }
};

class Memory {
 public:
  Memory() : _impl() {}

  /**
   * @brief createEntity
   * @param name representing the entity
   * @return the real id created
   */
  Entity createEntity(std::string const& name) {
    return _impl.createEntity(name);
  }

  /**
   * @brief set value to property of entity
   */
  template <typename T>
  bool set(Entity const& entity, std::string const& property, T const& val) {
    return _impl.set<T>(entity, property, val);
  }

  template <typename T>
  T* getEntityProperty(Entity const& entity, std::string const& property) {
    return _impl.getEntityProperty<T>(entity, property);
  }

  template <typename T>
  Container<T> getProperties(std::string const& property) {
    return _impl.getProperties<T>(property);
  }

  bool clearProperties(std::string const& property) {
    return _impl.clearProperties(property);
  }

  bool remove(Entity const& entity, std::string const& property) {
    return _impl.remove(entity, property);
  }

  template <typename T>
  Result process(std::string const& component,
                 Result updateFunc(Entity const& e, T const& comp)) {
    auto componentContainer = _impl.getProperties<T>(component);
    if (!componentContainer) {
      return error("unknown component: " + component);
    }
    auto compIt = componentContainer->begin();
    auto itEnd = componentContainer->end();
    for (; compIt != itEnd; ++compIt) {
      updateFunc(*compIt);
    }
    return ok();
  }

  template <typename T1, typename T2>
  Result process(
      std::string const& c1, std::string const& c2,
      std::function<Result(Entity const& e, T1 const& comp1, T2 const& comp2)>
          updateFunc) {
    auto container1 = _impl.getProperties<T1>(c1);
    if (!container1) {
      return error("unknown component: " + c1);
    }
    auto c1It = container1->begin();
    auto c1ItEnd = container1->end();

    auto container2 = _impl.getProperties<T2>(c2);
    if (!container2) {
      return error("unknown component: " + c2);
    }
    auto c2It = container2->begin();
    auto c2ItEnd = container2->end();

    while (c1It != c1ItEnd && c2It != c2ItEnd) {
      auto e1 = c1It->entity;
      auto e2 = c2It->entity;
      if (e1 < e2) {
        ++c1It;
        continue;
      }
      if (e2 < e1) {
        ++c2It;
        continue;
      }
      check_result(updateFunc(c1It->entity, c1It->value, c2It->value));
      ++c1It;
      ++c2It;
    }
    return ok();
  }

 private:
  BlackboardImpl _impl;
};

}  // namespace arty

#endif  // BLACKBOARD_HPP
