#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <algorithm>
#include <any>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
#include <variant>
#include <vector>

namespace arty {

using Entity = std::string;

template <typename T>
using Property = std::pair<std::string, T>;

// helper type for the visitor
template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

class Container {
 public:
  // CONTAINER PART
  template <typename T>
  using value_type = std::pair<std::string, T>;
  template <typename T>
  using sparse_storage_t = std::unordered_map<std::string, Property<T>>;
  template <typename T>
  using dense_storage_t = std::vector<Property<T>>;
  using pointer_type = std::shared_ptr<std::any>;

  enum ContainerType {
    Dense,
    Sparse,
  };

  Container() : _type(Sparse) {}

  bool remove(Entity const& e) { return _remover(e); }

  template <typename T>
  bool set(std::string const& e, T const& v) {
    if (!_ptr) {
      construct<T>();
    }
    if (!is<T>()) {
      return false;
    }
    auto it = Container::iterator_type<T>(this, e);
    if (it == it.end()) {
      return it.add(e, v);
    }
    it->second = v;
    return true;
  }

  template <typename T>
  T const* get(std::string const& e) const {
    if (!is<T>() || !_ptr) {
      return nullptr;
    }
    auto it = Iterator<T>(this, e);
    if (it != it.end()) {
      return &it->second;
    } else {
      return nullptr;
    }
  }

  template <typename T>
  bool is() const {
    T v;
    std::any helper(v);
    return _type_name == helper.type().name();
  }

 private:
  template <typename T>
  void construct() {
    if (_type_name.empty()) {
      T v;
      std::any helper(v);
      _type_name = helper.type().name();
    }
    if (!_ptr && _type == Sparse) {
      _ptr = std::make_shared<std::any>(sparse_storage_t<T>());
    } else if (!_ptr && _type == Dense) {
      _ptr = std::make_shared<std::any>(dense_storage_t<T>());
    }
    _remover = [&](Entity const& e) -> bool {
      auto it = Iterator<T>(this, e);
      return it.erase();
    };
  }

 private:
  ContainerType _type;
  pointer_type _ptr;
  std::string _type_name;
  std::function<bool(Entity const&)> _remover;

  // Iterator part
 public:
  template <typename T>
  class Iterator {
   public:
    using self_type = Iterator<T>;
    using value_type = std::pair<std::string, T>;
    using reference = value_type&;
    using pointer = value_type*;

    Iterator(Container const* container) {
      if (!container) {
        return;
      }
      makePointer(container);
      if (_var_ptr.index() == 0) {
        _var_curr = std::get<real_sparse_storage_t*>(_var_ptr)->begin();
      } else {
        _var_curr = std::get<real_dense_storage_t*>(_var_ptr)->begin();
      }
    }

    Iterator(Container* container) {
      if (!container) {
        return;
      }
      makePointer(container);
      if (_var_ptr.index() == 0) {
        _var_curr = std::get<real_sparse_storage_t*>(_var_ptr)->begin();
      } else {
        _var_curr = std::get<real_dense_storage_t*>(_var_ptr)->begin();
      }
    }

    Iterator(Container const* container, Entity const& e)
        : Iterator(container) {
      seek(e);
    }

    Iterator(Container* container, Entity const& e) : Iterator(container) {
      seek(e);
    }

    self_type begin() { return *this; }
    self_type end() const { return Iterator(_var_ptr); }

    bool operator!=(self_type const& other) const {
      return _var_curr != other._var_curr;
    }

    bool operator==(self_type const& other) const { return !(*this != other); }

    self_type& operator++() {
      std::visit([](auto&& arg) { ++arg; }, _var_curr);
      return *this;
    }
    reference operator*() {
      return std::visit(
          overloaded{
              [](sparse_it_t const& arg) -> reference { return arg->second; },
              [](dense_it_t const& arg) -> reference { return *arg; }},
          _var_curr);
    };
    pointer operator->() { return &**this; }  // *** EDIT

    bool add(Entity const& e, T const& v) {
      if (_var_ptr.index() == 0) {
        return std::get<real_sparse_storage_t*>(_var_ptr)
            ->emplace(e, value_type(e, v))
            .second;
      } else {
        std::get<real_dense_storage_t*>(_var_ptr)->emplace_back(e, v);
        return true;
      }
    }

    bool erase() {
      if (*this == end()) {
        return false;
      }
      if (_var_ptr.index() == 0) {
        std::get<real_sparse_storage_t*>(_var_ptr)->erase(
            std::get<sparse_it_t>(_var_curr));
        return true;
      } else {
        std::get<real_dense_storage_t*>(_var_ptr)->erase(
            std::get<dense_it_t>(_var_curr));
        return true;
      }
    }

   private:
    using real_sparse_storage_t = typename Container::sparse_storage_t<T>;
    using real_dense_storage_t = typename Container::dense_storage_t<T>;
    using sparse_it_t = typename real_sparse_storage_t::iterator;
    using dense_it_t = typename real_dense_storage_t::iterator;
    using underlying_type = std::variant<sparse_it_t, dense_it_t>;
    using variant_pointer =
        std::variant<real_sparse_storage_t*, real_dense_storage_t*>;

    Iterator(variant_pointer v) : _var_ptr(v) {
      if (_var_ptr.index() == 0) {
        _var_curr = std::get<real_sparse_storage_t*>(_var_ptr)->end();
      } else {
        _var_curr = std::get<real_dense_storage_t*>(_var_ptr)->end();
      }
    }

    void makePointer(Container const* container) {
      try {
        if (container->type() == Sparse) {
          _var_ptr = std::any_cast<typename Container::sparse_storage_t<T>>(
              container->ptr().get());
        } else {
          _var_ptr = std::any_cast<typename Container::dense_storage_t<T>>(
              container->ptr().get());
        }
      } catch (std::bad_any_cast const& err) {
        std::cerr << err.what() << std::endl;
      }
    }

    void seek(Entity const& e) {
      if (_var_ptr.index() == 0) {
        _var_curr = std::get<real_sparse_storage_t*>(_var_ptr)->find(e);
      } else {
        auto ptr = std::get<real_dense_storage_t*>(_var_ptr);
        _var_curr = std::find_if(ptr->begin(), ptr->end(),
                                 [e](auto&& p) { return p.first == e; });
      }
    }

    underlying_type _var_curr;
    variant_pointer _var_ptr;
  };

  ContainerType const& type() const { return _type; }
  pointer_type const& ptr() const { return _ptr; }

  template <typename T>
  using const_iterator_type = Iterator<const T>;
  template <typename T>
  using iterator_type = Iterator<T>;
};

using ContainerMap = std::unordered_map<std::string, Container>;
/*
class MultiStorageIterator {
 public:
  using value_type = std::vector<Container::iterator_type>;
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
template <typename T>
class ContainerWrapper {
 public:
  void add(Container container) { _storages.push_back(container); }

  MultiStorageIterator begin() const;
  MultiStorageIterator end() const;

  operator bool() const;

 private:
  std::vector<Container> _storages;
};
*/

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
    return _props.at(p).set(e, v);
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
    return _props.at(p).get<T>(e);
  }

  bool remove(std::string const& e, std::string const& p);
  bool rmEnt(std::string const& e);
  bool rmProp(std::string const& p);

  bool hasEnt(std::string const& name) const;
  bool hasProp(std::string const& name) const;

  void write(std::ostream& out) const;
  void load(std::istream const& in);

  // PROPERTIES ITERATION HELPERS
  template <typename T>
  Container::const_iterator_type<T> iterator(std::string const& p) const {
    return Container::const_iterator_type<T>(&_props.at(p));
  }

  template <typename T>
  Container::iterator_type<T> iterator(std::string const& p) {
    return Container::iterator_type<T>(&_props.at(p));
  }

  /*
  template <typename... Args>
  StorageWrapper props(Args... args) const {
    StorageWrapper wrapper;
    makeWrapper(wrapper, args...);
    return wrapper;
  }

 private:
  void makeWrapper(StorageWrapper& wrapper) const {}
  void makeWrapper(StorageWrapper& wrapper, std::string const& first) const {
    wrapper.add(prop(first));
  }
  template <typename... Args>
  void makeWrapper(StorageWrapper& wrapper, std::string const& first,
                   Args... args) const {
    wrapper.add(prop(first));
    makeWrapper(wrapper, args...);
  }
*/
 private:
  std::set<std::string> _entities;
  ContainerMap _props;
};

using Blackboard = Memory;

}  // namespace arty

#endif  // MEMORY_HPP
