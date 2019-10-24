#ifndef MATH_H
#define MATH_H

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <limits>

namespace arty {

template <typename T, int Rows, int Cols, class Derived>
class MatBase {
  static_assert(Rows > 0, "Rows must be positiv");
  static_assert(Cols > 0, "Cols must be positiv");

 public:
  static constexpr int rows = Rows;
  static constexpr int cols = Cols;
  static constexpr int size = Rows * Cols;

 protected:
  T arr[size];

 public:
  MatBase() {
    for (size_t j = 0; j < Cols; ++j) {
      for (size_t i = 0; i < Rows; ++i) {
        arr[i * Cols + j] = 0;
      }
    }
  }

  MatBase(T const& v) : MatBase() {
    for (int j = 0, i = 0; i < Rows && j < Cols; ++i, ++j) {
      arr[i * Cols + j] = v;
    }
  }

  MatBase(std::initializer_list<T> l) {
    assert(l.size() == size);
    auto it = l.begin();
    auto end = l.end();
    int i = 0;
    for (; it != end; ++i, ++it) {
      arr[i] = *it;
    }
  }

  MatBase(T const p[size]) { std::memcpy(arr, p, size); }

  T const& operator()(size_t i, size_t j) const {
    assert(i < rows);
    assert(j < cols);
    return arr[i * cols + j];
  }

  T& operator()(size_t i, size_t j) {
    assert(i < rows);
    assert(j < cols);
    return arr[i * cols + j];
  }

  T const& operator[](size_t i) const {
    assert(i < size);
    return arr[i];
  }

  Derived& operator+=(MatBase<T, Rows, Cols, Derived> const& other) {
    for (int i = 0; i < size; ++i) {
      arr[i] += other.arr[i];
    }
    return *reinterpret_cast<Derived*>(this);
    ;
  }

  MatBase<T, Rows, Cols, Derived>& operator-=(
      MatBase<T, Rows, Cols, Derived> const& other) {
    for (int i = 0; i < size; ++i) {
      arr[i] -= other.arr[i];
    }
    return *this;
  }

  template <typename S>
  Derived& operator*=(S const& scalar) {
    for (int i = 0; i < size; ++i) {
      arr[i] *= scalar;
    }
    return *reinterpret_cast<Derived*>(this);
  }

  Derived& operator*=(MatBase<T, Rows, Cols, Derived> const& r) {
    MatBase<T, Rows, Cols, Derived> res;
    for (size_t i = 0; i < Rows; ++i) {
      for (size_t j = 0; j < Cols; ++j) {
        for (size_t step = 0; step < Cols; ++step) {
          res(i, j) += (*this)(i, step) * r(step, j);
        }
      }
    }
    std::memcpy(this->arr, res.arr, sizeof(this->arr));
    return *reinterpret_cast<Derived*>(this);
  }

  T const* const ptr() const { return arr; }

  bool operator==(MatBase<T, Rows, Cols, Derived> const& r) const {
    return std::memcmp(this->arr, r.arr, sizeof(this->arr)) == 0;
  }

  bool operator!=(MatBase<T, Rows, Cols, Derived> const& r) const {
    return !(*this == r);
  }
};  // namespace arty

template <typename T, int Rows, int Cols, class Derived>
inline const Derived operator+(MatBase<T, Rows, Cols, Derived> l,
                               MatBase<T, Rows, Cols, Derived> const& r) {
  l += r;
  return *reinterpret_cast<Derived*>(&l);
}

template <typename T, int Rows, int Cols, class Derived>
inline const MatBase<T, Rows, Cols, Derived> operator-(
    MatBase<T, Rows, Cols, Derived> l,
    MatBase<T, Rows, Cols, Derived> const& r) {
  l -= r;
  return l;
}

template <typename T, int Rows, int Cols, class Derived>
inline Derived operator*(MatBase<T, Rows, Cols, Derived> l, T const& r) {
  l *= r;
  return *reinterpret_cast<Derived*>(&l);
}

template <typename T, int Rows, int Cols, class Derived>
inline const Derived operator*(MatBase<T, Rows, Cols, Derived> l,
                               MatBase<T, Rows, Cols, Derived> const& r) {
  l *= r;
  return *reinterpret_cast<Derived*>(&l);
}

template <typename T, int Rows, int Cols, class Derived>
inline T dot(MatBase<T, Rows, Cols, Derived> const& l,
             MatBase<T, Rows, Cols, Derived> const& r) {
  T res(0);
  for (int i = 0; i < l.size; ++i) {
    res += l[i] * r[i];
  }
  return res;
}

template <typename T, int Rows, int Cols, class Derived>
inline Derived normalize(MatBase<T, Rows, Cols, Derived> const& m) {
  T invsqrt = T(1) / std::sqrt(dot(m, m));
  MatBase<T, Rows, Cols, Derived> res = m * invsqrt;
  return *reinterpret_cast<Derived*>(&res);
}

template <typename T, int Rows, int Cols, class Derived>
inline Derived transpose(MatBase<T, Rows, Cols, Derived> const& m) {
  return transpose(*reinterpret_cast<Derived const*>(&m));
}

template <typename T, int Rows, int Cols>
class Mat : public MatBase<T, Rows, Cols, Mat<T, Rows, Cols>> {
 public:
  using Base = MatBase<T, Rows, Cols, Mat<T, Rows, Cols>>;
  Mat() : Base() {}
  Mat(T const& v) : Base(v) {}
  Mat(std::initializer_list<T> l) : Base(l) {}
  Mat(T p[Base::size]) : Base(p) {}
};

template <typename T, int Rows, int Cols>
inline Mat<T, Cols, Rows> transpose(Mat<T, Rows, Cols> const& m) {
  Mat<T, Cols, Rows> res;
  for (std::size_t i = 0; i < Rows; ++i) {
    for (std::size_t j = 0; j < Cols; ++j) {
      res(j, i) = m(i, j);
    }
  }
  return res;
}

template <typename T>
using Mat3x3 = Mat<T, 3, 3>;
using Mat3x3f = Mat3x3<float>;

template <typename T>
using Mat4x4 = Mat<T, 4, 4>;
using Mat4x4f = Mat4x4<float>;

template <typename T, int Rows>
class Vec : public MatBase<T, Rows, 1, Vec<T, Rows>> {
 public:
  using Base = MatBase<T, Rows, 1, Vec<T, Rows>>;
  Vec() : Base() {}
  Vec(T const& v) : Base(v) {}
  Vec(std::initializer_list<T> l) : Base(l) {}
  Vec(T p[Base::size]) : Base(p) {}
  Vec(Base const& other) : Base(other) {}

  T& x() { return Base::arr[0]; }
  T const& x() const { return Base::arr[0]; }
  T& y() {
    static_assert(Base::size > 1, "Size <= 1");
    return Base::arr[1];
  }
  T const& y() const {
    static_assert(Base::size > 1, "Size <= 1");
    return Base::arr[1];
  }
  T& z() {
    static_assert(Base::size > 2, "Size <= 2");
    return Base::arr[2];
  }
  T const& z() const {
    static_assert(Base::size > 2, "Size <= 2");
    return Base::arr[2];
  }
  T& w() {
    static_assert(Base::size > 3, "Size <= 3");
    return Base::arr[3];
  }
  T const& w() const {
    static_assert(Base::size > 3, "Size <= 3");
    return Base::arr[3];
  }
};

template <typename T>
using Vec3 = Vec<T, 3>;
using Vec3f = Vec3<float>;

template <typename T>
Vec3<T> inline cross(Vec3<T> const& l, Vec3<T> const& r) {
  return Vec3<T>{
      l[1] * r[2] - r[1] * l[2],  //
      l[2] * r[0] - r[2] * l[0],  //
      l[0] * r[1] - r[0] * l[1],  //
  };
}

template <typename T>
class Quat : public Vec<T, 4> {
 public:
  using Base = Vec<T, 4>;
  Quat() : Base({0.f, 0.f, 0.f, 1.f}) {}
  Quat(std::initializer_list<T> l) : Base(l) {}
  Quat(T p[Base::size]) : Base(p) {}

  Vec3<T> operator*(Vec3<T> const& v) {
    T t2 = Base::arr[0] * Base::arr[1];
    T t3 = Base::arr[0] * Base::arr[2];
    T t4 = Base::arr[0] * Base::arr[3];
    T t5 = -Base::arr[1] * Base::arr[1];
    T t6 = Base::arr[1] * Base::arr[2];
    T t7 = Base::arr[1] * Base::arr[3];
    T t8 = -Base::arr[2] * Base::arr[2];
    T t9 = Base::arr[2] * Base::arr[3];
    T t10 = -Base::arr[3] * Base::arr[3];
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

template <typename T>
inline T radians(T const& degree) {
  return degree * static_cast<T>(0.01745329251994329576923690768489);
}

template <typename T>
inline Mat4x4<T> perspective(T const& fov, T const& aspect, T const& znear,
                             T const& zfar) {
  assert(std::abs(aspect - std::numeric_limits<T>::epsilon()) >
         static_cast<T>(0));

  T const tanHalfFovy = std::tan(fov / static_cast<T>(2));
  Mat4x4<T> mat;
  mat(0, 0) = static_cast<T>(1) / (aspect * tanHalfFovy);
  mat(1, 1) = static_cast<T>(1) / (tanHalfFovy);
  mat(2, 2) = -(zfar + znear) / (zfar - znear);
  mat(3, 2) = -static_cast<T>(1);
  mat(2, 3) = -(static_cast<T>(2) * zfar * znear) / (zfar - znear);
  return mat;
}

template <typename T>
inline Mat4x4<T> lookAt(Vec3<T> const& eye, Vec3<T> const& center,
                        Vec3<T> const& up) {
  Vec3<T> const f(normalize(center - eye));
  Vec3<T> const s(normalize(cross(f, up)));
  Vec3<T> const u(cross(s, f));

  Mat4x4<T> result(1);
  result(0, 0) = s.x();
  result(0, 1) = s.y();
  result(0, 2) = s.z();
  result(1, 0) = u.x();
  result(1, 1) = u.y();
  result(1, 2) = u.z();
  result(2, 0) = -f.x();
  result(2, 1) = -f.y();
  result(2, 2) = -f.z();
  result(0, 3) = -dot(s, eye);
  result(1, 3) = -dot(u, eye);
  result(2, 3) = dot(f, eye);
  return result;
}

template <typename T, int Rows, int Cols, class Derived>
static std::ostream& operator<<(
    std::ostream& out, arty::MatBase<T, Rows, Cols, Derived> const& mat) {
  for (std::size_t i = 0; i < Rows; ++i) {
    out << "\n|";
    for (std::size_t j = 0; j < Cols; ++j) {
      out << " " << mat(i, j);
    }
    out << "|";
  }
  return out;
}

template <typename T>
inline Mat4x4<T> translation(T const& x, T const& y, T const& z) {
  Mat4x4<T> res(static_cast<T>(1.f));
  res(0, 3) = x;
  res(1, 3) = y;
  res(2, 3) = z;
  return res;
}

template <typename T>
inline Mat4x4<T> translation(Vec3<T> const& t) {
  Mat4x4<T> res(static_cast<T>(1.f));
  res(0, 3) = t.x();
  res(1, 3) = t.y();
  res(2, 3) = t.z();
  return res;
}

template <typename T>
inline Mat4x4<T> rotation(T const& a, T const& b, T const& c) {
  Mat4x4<T> res(static_cast<T>(1.f));
  T c1 = std::cos(a);
  T c2 = std::cos(b);
  T c3 = std::cos(c);
  T s1 = std::sin(a);
  T s2 = std::sin(b);
  T s3 = std::sin(c);
  res(0, 0) = c2 * c3;
  res(0, 1) = -c2 * s3;
  res(0, 2) = s2;
  res(1, 0) = c1 * s3 + c3 * s1 * s2;
  res(1, 1) = c1 * c3 - s1 * s2 * s3;
  res(1, 2) = -c2 * s1;
  res(2, 0) = s1 * s3 - c1 * c3 * s2;
  res(2, 1) = c3 * s1 + c1 * s2 * s3;
  res(2, 2) = c1 * c2;
  return res;
}

}  // namespace arty

#endif  // MATH_H
