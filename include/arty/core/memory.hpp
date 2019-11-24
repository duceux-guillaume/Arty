#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <algorithm>
#include <any>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

namespace arty {

class Any {
 public:
  Any(std::string const& key) : _key(key), _val() {}

  template <typename T>
  Any(std::string const& key, T const& v) : _key(key), _val(v) {}

  std::string const& key() const { return _key; }
  std::any const& val() const { return _val; }

  bool operator==(Any const& other) const { return _key == other.key(); }
  bool operator<(Any const& other) const { return _key < other.key(); }

  template <typename T>
  bool operator==(T const& other) const {
    if (!check(other)) {
      return false;
    }
    return std::any_cast<T>(_val) == other;
  }

  template <typename T>
  bool set(T val) {
    std::any tmp(val);
    if (_val.type().name() == tmp.type().name()) {
      std::swap(_val, tmp);
      return true;
    }
    return false;
  }

  template <typename T>
  T get() {
    try {
      return std::any_cast<T>(_val);
    } catch (std::bad_any_cast const& err) {
      return T();
    }
  }

  template <typename T>
  bool check(T val) const {
    if (!_val.has_value()) {
      return true;
    }
    std::any tmp(val);
    return _val.type().name() == tmp.type().name();
  }

 private:
  std::string _key;
  std::any _val;
};

}  // namespace arty

template <>
class std::hash<arty::Any> {
 public:
  size_t operator()(arty::Any const& s) const {
    return std::hash<std::string>()(s.key());
  }
};

namespace arty {

struct TypeHelper {
  template <typename T>
  static TypeHelper make();
};

using SparseStorage = std::set<Any>;
using PropMap = std::unordered_map<std::string, SparseStorage>;
using HelperMap = std::unordered_map<std::string, TypeHelper>;

class MultiStorageIterator {
 public:
  using value_type = std::vector<SparseStorage::const_iterator>;
  using reference = value_type&;
  using pointer = value_type*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  MultiStorageIterator() = default;
  MultiStorageIterator(value_type const& b) : _currs(b) {}

  void next();

  bool operator!=(MultiStorageIterator const& other) const;

  MultiStorageIterator& operator++() {
    next();
    return *this;
  }
  reference operator*() { return _currs; }

 private:
  value_type _currs;
};

class StorageWrapper {
 public:
  void add(SparseStorage const* storage) { _storages.push_back(storage); }

  MultiStorageIterator begin() const;
  MultiStorageIterator end() const;

  operator bool() const;

 private:
  std::vector<SparseStorage const*> _storages;
};

class Memory {
 public:
  /**
   * @brief newEntity
   * @param name representing the entity
   * @return true is success
   */
  bool newEnt(std::string const& name);
  std::string genEnt(std::string const& type);

  /**
   * @brief newProp
   * @param name representing the property
   * @return true is success
   */
  bool newProp(std::string const& name);

  /**
   * @brief set value to property of entity
   */
  template <typename T>
  bool set(std::string const& e, std::string const& p, T const& v) {
    if (!hasEnt(e)) {
      return false;
    }
    if (!hasProp(p)) {
      return false;
    }
    auto& storage = _props.at(p);
    auto it = storage.find(e);
    if (it == storage.end()) {
      // first time, we need to set our io funcs
      _writers.emplace(p, [](std::ostream& out, Any const& v) {
        out << v.key() << ":" << std::any_cast<T>(v.val());
      });
      storage.emplace(e, v);
      return true;
    }
    if (it->check(v)) {
      storage.erase(e);
      storage.emplace(e, v);
      return true;
    }
    return false;
  }

  bool set(std::string const& e, std::string const& p, const char* v) {
    return set(e, p, std::string(v));
  }

  /**
   * @brief get value of property of entity
   */
  template <typename T>
  T const* get(std::string const& e, std::string const& p) const {
    if (!hasEnt(e)) {
      return nullptr;
    }
    if (!hasProp(p)) {
      return nullptr;
    }
    try {
      auto const& storage = _props.at(p);
      auto it = storage.find(e);
      if (it == storage.end()) {
        return nullptr;
      }
      if (!it->check(T())) {
        return nullptr;
      }
      return std::any_cast<T>(&it->val());
    } catch (std::bad_any_cast const& err) {
      return nullptr;
    }
  }

  bool remove(std::string const& e, std::string const& p);
  bool rmEnt(std::string const& e);
  bool rmProp(std::string const& p);

  bool hasEnt(std::string const& name) const;
  bool hasProp(std::string const& name) const;

  void write(std::ostream& out) const;
  void load(std::istream const& in);

  // PROPERTIES ITERATION HELPERS
  SparseStorage const* prop(std::string const& p) const;
  template <typename... Args>
  StorageWrapper props(Args... args) const {
    StorageWrapper wrapper;
    makeWrapper(wrapper, args...);
    return wrapper;
  }

 private:
  void makeWrapper(StorageWrapper& /*wrapper*/) const {}
  void makeWrapper(StorageWrapper& wrapper, std::string const& first) const {
    wrapper.add(prop(first));
  }
  template <typename... Args>
  void makeWrapper(StorageWrapper& wrapper, std::string const& first,
                   Args... args) const {
    wrapper.add(prop(first));
    makeWrapper(wrapper, args...);
  }

 private:
  std::set<std::string> _entities;
  PropMap _props;
  using WriterHelper = std::function<void(std::ostream&, Any const&)>;
  using ParserHelper = std::function<Any(std::string const&)>;
  std::unordered_map<std::string, WriterHelper> _writers;
  std::unordered_map<std::string, WriterHelper> _parsers;
};

}  // namespace arty

#endif  // MEMORY_HPP
