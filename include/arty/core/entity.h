#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>

namespace arty {

class Entity {
 public:
  using id_t = uint64_t;

  Entity(std::string const &name) : _id(_next), _name(name) { _next++; }

  bool operator==(Entity const &e) const { return _id == e._id; }

  bool operator!=(Entity const &e) const { return !(*this == e); }

  bool operator<(Entity const &e) const { return _id < e._id; }

  std::string const &name() const { return _name; }

  id_t const &id() const { return _id; }

 private:
  id_t _id;
  std::string _name;
  static id_t _next;
};

static std::ostream &operator<<(std::ostream &os, arty::Entity const &r) {
  os << r.name() << r.id();
  return os;
}

}  // namespace arty

#endif  // ENTITY_H
