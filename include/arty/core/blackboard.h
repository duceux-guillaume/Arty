#ifndef BLACKBOARD_H
#define BLACKBOARD_H

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
    } while (!_vector_curr->entity.isValid() && _vector_curr != _vector_end);
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
    if (_buffer.size() < entity.id) {
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

 private:
  std::vector<Property<T>> _buffer;
  std::size_t _count;
};

using BaseStorage = Ptr<IStorage>;
template <typename T>
using DerivedStorage = Ptr<RealStorage<T>>;

class Blackboard {
 public:
  Blackboard() : _properties(), _entities(), _uuid(0) {}

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
  void set(Entity const& entity, std::string const& property, T const& val) {
    if (_properties.count(property) == 0) {
      _properties[property] = BaseStorage(new RealStorage<T>());
    }
    auto ptr = getProperties<T>(property);
    ptr->set(entity, val);
  }

  template <typename T>
  T* getEntityProperty(Entity const& entity, std::string const& property) {
    auto ptr = getProperties<T>(property);
    return ptr->get(entity);
  }

  template <typename T>
  DerivedStorage<T> getProperties(std::string const& property) {
    return pointer_cast<T>(_properties[property]);
  }

  Result clearProperties(std::string const& property) {
    if (_properties.count(property)) {
      _properties[property]->clear();
      return ok();
    }
    return error(std::string("unknown property: ") + property);
  }

 private:
  std::map<std::string, Ptr<IStorage>> _properties;
  std::set<Entity> _entities;
  std::size_t _uuid;

  template <typename T>
  DerivedStorage<T> pointer_cast(BaseStorage r) {
    auto p = static_cast<typename DerivedStorage<T>::element_type*>(r.get());
    return DerivedStorage<T>(r, p);
  }
};

struct Transform {
  Vec3f position;
  Quatf rotation;
  Vec3f scale;

  Mat4x4f toMat() const {
    Mat4x4f tf;
    float qx = rotation.x();
    float qy = rotation.y();
    float qz = rotation.z();
    float qw = rotation.w();
    float qx2 = qx * qx;
    float qy2 = qy * qy;
    float qz2 = qz * qz;
    tf(0, 0) = 1.f - (2 * qy2 + 2 * qz2);
    tf(1, 0) = 2 * qx * qy + 2 * qz * qw;
    tf(2, 0) = 2 * qx * qz - 2 * qy * qw;
    tf(0, 1) = 2 * qx * qy - 2 * qz * qw;
    tf(1, 1) = 1 - 2 * qx2 - 2 * qz2;
    tf(2, 1) = 2 * qy * qz + 2 * qx * qw;
    tf(0, 2) = 2 * qx * qz + 2 * qy * qw;
    tf(1, 2) = 2 * qy * qz - 2 * qx * qw;
    tf(2, 2) = 1 - 2 * qx2 - 2 * qy2;
    tf(0, 3) = position.x();
    tf(1, 3) = position.y();
    tf(2, 3) = position.z();
    tf(3, 3) = 1.f;
    return tf;
  }

  Transform() : position(), rotation(), scale() {}
  Transform(Vec3f pos) : position(pos), rotation(), scale() {}
};

}  // namespace arty

#endif  // BLACKBOARD_H
