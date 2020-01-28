#ifndef GEOMETRY_IMPL_HPP
#define GEOMETRY_IMPL_HPP

#include <arty/core/geometry.hpp>

namespace arty {

template <typename T, int Dim>
typename Transform<T, Dim>::matrix_type Transform<T, Dim>::toMat() const {
  matrix_type tf;
  tf.setBlock(0, 0, _rotation);
  tf.setBlock(0, dimension, _translation);
  tf(dimension, dimension) = value_type(1);
  return tf;
}

template <typename T, int Dim>
void Transform<T, Dim>::fromMat(const Transform<T, Dim>::matrix_type& m) {
  m.copyBlockTo(0, dimension, _translation);
  m.copyBlockTo(0, 0, _rotation);
}

template <typename T, int Dim>
typename Edge<T, Dim>::vector_type Edge<T, Dim>::project(
    Edge<T, Dim>::vector_type const& p) {
  Line<T, Dim> line(_a, _b);
  T coeff = line.dirCoeff(p);
  if (coeff <= 0) {
    return _a;
  }
  if (coeff >= 1.f) {
    return _b;
  }
  return _a + line.direction() * coeff;
}

template <typename T, int Dim>
bool Circle<T, Dim>::intersect(Circle<T, Dim> const& other) {
  float dist = (_center - other._center).normsqr();
  return dist <= _sqrRadius + other._sqrRadius;
}

template <typename T, int Dim>
bool Circle<T, Dim>::contains(Circle<T, Dim>::vector_type const& pt) {
  float dist = (_center - pt).normsqr();
  return dist <= _sqrRadius;
}

}  // namespace arty

#endif  // GEOMETRY_IMPL_HPP
