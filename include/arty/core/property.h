#ifndef PROPERTY_H
#define PROPERTY_H

#include <arty/core/entity.h>

namespace arty {

template <typename T>
class Property {
 public:
  Property(Entity const& entity, T const& val) : _entity(entity), _val(val) {}
  T& val() { return _val; }
  T const& val() const { return _val; }

 private:
  T _val;
  Entity _entity;
};

}  // namespace arty

#endif  // PROPERTY_H
