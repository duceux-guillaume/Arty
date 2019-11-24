#include <arty/core/memory.hpp>

namespace arty {

bool Memory::newEnt(const std::string &name) {
  return _entities.emplace(name).second;
}

bool Memory::newProp(const std::string &name) {
  return _props.emplace(name, SparseStorage()).second;
}

bool Memory::remove(const std::string &e, const std::string &p) {
  auto prop = _props.find(p);
  if (prop == _props.end()) {
    return false;
  }
  auto ent = prop->second.find(e);
  if (ent == prop->second.end()) {
    return false;
  }
  prop->second.erase(ent);
  return true;
}

bool Memory::rmEnt(const std::string &e) {
  if (!hasEnt(e)) {
    return false;
  }
  _entities.erase(e);
  for (auto &props : _props) {
    props.second.erase(e);
  }
  return true;
}

bool Memory::rmProp(const std::string &p) {
  if (!hasProp(p)) {
    return false;
  }
  _props.erase(p);
  return true;
}

SparseStorage const *Memory::prop(const std::string &p) const {
  if (!hasProp(p)) {
    return nullptr;
  }
  return &_props.at(p);
}

bool Memory::hasEnt(const std::string &name) const {
  return _entities.find(name) != _entities.end();
}

bool Memory::hasProp(const std::string &name) const {
  return _props.find(name) != _props.end();
}

void Memory::write(std::ostream &out) const {
  for (auto const &item : _props) {
    out << item.first << " ";
    if (_writers.count(item.first) > 0) {
      auto writer = _writers.at(item.first);
      for (auto const &prop : item.second) {
        writer(out, prop);
        out << " ";
      }
    }
    out << std::endl;
  }
}

void MultiStorageIterator::next() {
  // We move first, and then we make other move if they are inferior
  // If firts is the one inferior to others, we next() again
  if (_currs.size() == 0) {
    return;
  }
  ++_currs[0];
  if (_currs.size() == 1) {
    return;
  }
  auto key = _currs[0]->key();
  std::transform(_currs.begin() + 1, _currs.end(), _currs.begin() + 1,
                 [key](value_type::value_type &val) {
                   while (val->key() < key) {
                     ++val;
                   }
                   return val;
                 });
  auto greater = std::any_of(
      _currs.begin() + 1, _currs.end(),
      [key](value_type::value_type const &val) { return val->key() > key; });
  if (greater) {
    next();
  }
}

bool MultiStorageIterator::operator!=(const MultiStorageIterator &other) const {
  if (_currs.size() != other._currs.size()) {
    return true;
  }
  return !std::equal(_currs.begin(), _currs.end(), other._currs.begin());
}

arty::StorageWrapper::operator bool() const {
  if (_storages.size() == 0) {
    return false;
  }
  return std::all_of(_storages.begin(), _storages.end(),
                     [](SparseStorage const *ptr) { return ptr; });
}

MultiStorageIterator StorageWrapper::begin() const {
  if (*this) {
    std::vector<SparseStorage::const_iterator> beginnings;
    return MultiStorageIterator(beginnings);
  }
  return MultiStorageIterator();
}

MultiStorageIterator StorageWrapper::end() const {
  if (*this) {
    std::vector<SparseStorage::const_iterator> ends;
    return MultiStorageIterator(ends);
  }
  return MultiStorageIterator();
}

}  // namespace arty
