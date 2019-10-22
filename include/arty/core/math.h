#ifndef MATH_H
#define MATH_H

#include <cassert>
#include <cstring>
#include <initializer_list>

namespace arty {

template <typename T, int Rows, int Cols>
class Mat {
 private:
  T _arr[Rows * Cols];

 public:
  static constexpr int rows = Rows;
  static constexpr int cols = Cols;
  static constexpr int size = Rows * Cols;

  Mat(std::initializer_list<T> l) {
    assert(l.size() == size);
    auto it = l.begin();
    auto end = l.end();
    int i = 0;
    for (; it != end; ++i, ++it) {
      _arr[i] = *it;
    }
  }

  Mat(T p[size]) { std::memcpy(_arr, p, size); }

  T const& operator[](size_t i) const {
    assert(i < size);
    return _arr[i];
  }
  T const& operator()(size_t i, size_t j) const {
    assert(i < rows);
    assert(j < cols);
    return _arr[i * cols + j];
  }

  Mat<T, Rows, Cols>& operator+=(Mat<T, Rows, Cols> const& other) {
    for (int i = 0; i < size; ++i) {
      _arr[i] += other._arr[i];
    }
    return *this;
  }
};

template <typename T, int Rows, int Cols>
inline const Mat<T, Rows, Cols> operator+(Mat<T, Rows, Cols> l,
                                          Mat<T, Rows, Cols> const& r) {
  l += r;
  return l;
}

template <typename T>
using Vec3 = Mat<T, 3, 1>;
using Vec3f = Vec3<float>;

template <typename T>
using Mat3x3 = Mat<T, 3, 3>;
using Mat3x3f = Mat3x3<float>;

template <typename T>
class Quat {
 public:
  static constexpr int size = 4;

 private:
  union {
    T _arr[size];
  };

 public:
  Quat(std::initializer_list<T> l) {
    assert(l.size() == size);
    auto it = l.begin();
    auto end = l.end();
    int i = 0;
    for (; it != end; ++i, ++it) {
      _arr[i] = *it;
    }
  }

  Quat(T p[size]) { std::memcpy(_arr, p, size); }

  Vec3<T> operator*(Vec3<T> const& v) {
    T t2 = _arr[0] * _arr[1];
    T t3 = _arr[0] * _arr[2];
    T t4 = _arr[0] * _arr[3];
    T t5 = -_arr[1] * _arr[1];
    T t6 = _arr[1] * _arr[2];
    T t7 = _arr[1] * _arr[3];
    T t8 = -_arr[2] * _arr[2];
    T t9 = _arr[2] * _arr[3];
    T t10 = -_arr[3] * _arr[3];
    T v1new =
        2 * ((t8 + t10) * v[0] + (t6 - t4) * v[1] + (t3 + t7) * v[2]) + v[0];
    T v2new =
        2 * ((t4 + t6) * v[0] + (t5 + t10) * v[1] + (t9 - t2) * v[2]) + v[1];
    T v3new =
        2 * ((t7 - t3) * v[0] + (t2 + t9) * v[1] + (t5 + t8) * v[2]) + v[2];
    return Vec3<T>{v1new, v2new, v3new};
  }
};

using Quatf = Quat<float>;

}  // namespace arty

#endif  // MATH_H
