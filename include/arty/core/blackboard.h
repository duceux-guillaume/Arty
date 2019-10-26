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

struct Camera {
  Mat4x4f projection;
  Mat4x4f view;
};

class BasePropertyStorage {
 public:
 protected:
};

template <typename T>
class PropertyStorage : public BasePropertyStorage {
 public:
  void set(std::string const& entity, T const& val) {
    T* ptr = get(entity);
    if (!ptr) {
      _buffer.push_back(val);
      _mapping.emplace(entity, _buffer.size() - 1);
    } else {
      *ptr = val;
    }
  }

  std::vector<T> const& buffer() const { return _buffer; }
  std::vector<T>& buffer() { return _buffer; }

  T* get(std::string const& entity) {
    if (_mapping.count(entity) == 0) {
      return nullptr;
    } else {
      std::size_t index = _mapping[entity];
      assert(index < _buffer.size());
      return &_buffer[index];
    }
  }

 private:
  std::vector<T> _buffer;
  std::unordered_map<std::string, std::size_t> _mapping;
};

using BaseStorage = Ptr<BasePropertyStorage>;
template <typename T>
using DerivedStorage = Ptr<PropertyStorage<T>>;

class Blackboard {
 public:
  /**
   * @brief createEntity
   * @param name representing the entity
   * @return the real id created
   */
  std::string createEntity(std::string const& name) {
    if (_entities.count(name) == 0) {
      _entities.insert(name);
      return name;
    }
    std::string newname = name + std::to_string(_uuid);
    ++_uuid;
    _entities.insert(newname);
    return newname;
  }

  template <typename T>
  void set(std::string const& entity, std::string const& property,
           T const& val) {
    if (_properties.count(property) == 0) {
      _properties[property] = BaseStorage(new PropertyStorage<T>());
    }
    auto ptr = getProperties<T>(property);
    ptr->set(entity, val);
  }

  template <typename T>
  T* getEntityProperty(std::string const& entity, std::string const& property) {
    auto ptr = getProperties<T>(property);
    return ptr->get(entity);
  }

  template <typename T>
  DerivedStorage<T> getProperties(std::string const& property) {
    return pointer_cast<T>(_properties[property]);
  }

  void setCamera(Camera const& cam) { _camera = cam; }
  Camera const& getCamera() { return _camera; }

 private:
  std::map<std::string, Ptr<BasePropertyStorage>> _properties;
  std::set<std::string> _entities;
  Camera _camera;
  std::size_t _uuid;

  template <typename T>
  DerivedStorage<T> pointer_cast(BaseStorage r) {
    auto p = static_cast<typename DerivedStorage<T>::element_type*>(r.get());
    return DerivedStorage<T>(r, p);
  }
};

struct Transform {
  std::string entity;
  Vec3f position;
  Quatf rotation;
  Vec3f scale;
};

struct Shader {
  std::string vertex;
  std::string fragment;
  std::string textureFile;
  uint32_t program;
  uint32_t matrixId;
  uint32_t viewId;
  uint32_t modelId;
  uint32_t lightId;
  uint32_t texture;
  uint32_t textureId;
};

struct BufferVec3f {
  std::string file;
  uint32_t array;
  uint32_t vbo;
  std::vector<Vec3f> buffer;
};

struct BufferVec2f {
  std::string file;
  uint32_t vbo;
  std::vector<Vec2f> buffer;
};

}  // namespace arty

#endif  // BLACKBOARD_H
