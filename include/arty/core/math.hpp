
#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>

#define MAT_TEMP template <typename T, int Rows, int Cols>
#define MAT_TYPE Mat<T, Rows, Cols>

namespace arty {

#ifdef __linux__
namespace details {

template <bool...>
struct bool_pack;

template <bool... v>
using all_true = std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>;

template <typename T, class... Args>
using EnableIfConvertible =
    std::enable_if_t<all_true<std::is_convertible<Args, T>{}...>{}>;

#define VARIADIC_ARG \
  typename... Args, class = details::EnableIfConvertible<T, Args...>
#define VARIADIC_TEMP \
  template <typename... Args, class = details::EnableIfConvertible<T, Args...>>

}  // namespace details
#else
#define VARIADIC_ARG typename... Args
#define VARIADIC_TEMP template <typename... Args>
#endif

MAT_TEMP
class Mat {
  static_assert(Rows > 0, "Rows must be positiv");
  static_assert(Cols > 0, "Cols must be positiv");

 public:
  static constexpr int rows = Rows;
  static constexpr int cols = Cols;
  static constexpr int size = Rows * Cols;
  static constexpr bool is_vector = rows == 1 || cols == 1;
  static constexpr bool is_square = rows == cols;
  using self_type = Mat<T, Rows, Cols>;
  using value_type = T;
  using transpose_type = Mat<T, Cols, Rows>;
  using col_type = Mat<T, Rows, 1>;
  using row_type = Mat<T, 1, Cols>;
  using iterator_type = value_type*;
  using const_iterator_type = value_type const*;

 protected:
  value_type _arr[size];

 public:
  Mat() : _arr{0} {}
  Mat(std::initializer_list<T> l) {
    assert(l.size() == size);
    auto it = l.begin();
    auto end = l.end();
    int i = 0;
    for (; it != end; ++i, ++it) {
      _arr[i] = *it;
    }
  }
  Mat(self_type const& o) = default;
  Mat(self_type&& o) = default;
  self_type& operator=(self_type const& o) = default;
  self_type& operator=(self_type&& o) = default;

  template <typename G>
  explicit Mat(Mat<G, rows, cols> const& other) {
    setBlock(0, 0, other);
  }

  // SPECIAL CONSTRUCTOR
  static self_type diagonal(T const& v) {
    self_type m;
    for (size_t i = 0; i < Rows && i < Cols; ++i) {
      m(i, i) = v;
    }
    return m;
  }
  static self_type identity() { return diagonal(T(1)); }
  static self_type zero() { return self_type(); }
  static self_type all(value_type const& v) {
    self_type m;
    m.forEach([v](value_type& e) { e = v; });
    return m;
  }

  // GETTERS
  T const& operator()(size_t i, size_t j) const {
    assert(i < rows);
    assert(j < cols);
    return _arr[i * cols + j];
  }

  T& operator()(size_t i, size_t j) {
    assert(i < rows);
    assert(j < cols);
    return _arr[i * cols + j];
  }

  T const& operator[](size_t i) const {
    assert(i < size);
    return _arr[i];
  }

  T& operator[](size_t i) {
    assert(i < size);
    return _arr[i];
  }

  // BLOCKS
  col_type col(std::size_t j) const {
    assert(j < cols);
    col_type r;
    for (std::size_t i = 0; i < rows; ++i) {
      r[i] = (*this)(i, j);
    }
    return r;
  }

  row_type row(std::size_t i) const {
    assert(i < rows);
    row_type r;
    for (std::size_t j = 0; j < cols; ++j) {
      r[j] = (*this)(i, j);
    }
    return r;
  }

  /**
   * @brief extract a block from matrix
   */
  template <typename N, int R, int C>
  Mat<N, R, C> block(std::size_t i, std::size_t j) const {
    static_assert(R <= Rows && C <= Cols,
                  "block boundaries are outside of matrix");
    static_assert(R > 0 && C > 0, "a matrix cannot have 0 dimension");
    assert(i + R - 1 < rows);
    assert(j + C - 1 < cols);
    Mat<value_type, R, C> res;
    for (std::size_t k = 0; k < R; ++k) {
      for (std::size_t l = 0; l < C; ++l) {
        res(k, l) = static_cast<N>((*this)(i + k, l + j));
      }
    }
    return res;
  }

  template <typename N, int R, int C>
  void copyBlockTo(std::size_t i, std::size_t j, Mat<N, R, C>& res) const {
    static_assert(R <= Rows && C <= Cols,
                  "block boundaries are outside of matrix");
    static_assert(R > 0 && C > 0, "a matrix cannot have 0 dimension");
    assert(i + R - 1 < rows);
    assert(j + C - 1 < cols);
    for (std::size_t k = 0; k < R; ++k) {
      for (std::size_t l = 0; l < C; ++l) {
        res(k, l) = static_cast<N>((*this)(i + k, l + j));
      }
    }
  }

  void setCol(std::size_t j, col_type const& col) {
    assert(j < cols);
    for (std::size_t i = 0; i < rows; ++i) {
      (*this)(i, j) = col[i];
    }
  }

  void setRow(std::size_t i, row_type const& row) {
    assert(i < rows);
    for (std::size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = row[j];
    }
  }

  template <typename G, int R, int C>
  void setBlock(std::size_t i, std::size_t j, Mat<G, R, C> const& block) {
    static_assert(R <= Rows && C <= Cols, "block is bigger than matrix");
    static_assert(R > 0 && C > 0, "a matrix cannot have 0 dimension");
    assert(i + R - 1 < rows);
    assert(j + C - 1 < cols);
    for (std::size_t k = 0; k < R; ++k) {
      for (std::size_t l = 0; l < C; ++l) {
        (*this)(i + k, l + j) = static_cast<value_type>(block(k, l));
      }
    }
  }

  // VARIADIC CONSTRUCTION
 private:
  void setAt(std::size_t /*i*/) {}
  void setAt(std::size_t i, value_type const& first) {
    assert(i < size);
    _arr[i] = first;
  }
  VARIADIC_TEMP
  void setAt(std::size_t i, value_type const& first, Args... args) {
    _arr[i] = first;
    setAt(i + 1, args...);
  }

  VARIADIC_TEMP
  void setCols(std::size_t j, col_type const& col, Args... args) {
    assert(j < cols);
    setCol(j, col);
    setCols(j + 1, args...);
  }

  VARIADIC_TEMP
  void setRows(std::size_t j, row_type const& row, Args... args) {
    assert(j < rows);
    setRow(j, row);
    setRows(j + 1, args...);
  }

 public:
  VARIADIC_TEMP
  void set(Args... args) { setAt(0, args...); }

  VARIADIC_TEMP
  explicit Mat(Args... args) : Mat() { set(args...); }

  VARIADIC_TEMP
  static self_type fromCols(col_type const& col, Args... args) {
    self_type m;
    m.setCol(0, col);
    m.setCols(1, args...);
    return m;
  }

  VARIADIC_TEMP
  static self_type fromRows(row_type const& row, Args... args) {
    self_type m;
    m.setRow(0, row);
    m.setRows(1, args...);
    return m;
  }

  // OPERATORS
  self_type& operator+=(self_type const& other) {
    for (int i = 0; i < size; ++i) {
      _arr[i] += other._arr[i];
    }
    return *this;
  }

  self_type& operator-=(self_type const& other) {
    for (int i = 0; i < size; ++i) {
      _arr[i] -= other._arr[i];
    }
    return *this;
  }

  self_type operator-() const {
    return this->apply([](T const& f) { return -f; });
  }

  template <typename S>
  self_type& operator*=(S const& scalar) {
    for (int i = 0; i < size; ++i) {
      _arr[i] *= scalar;
    }
    return *this;
  }

  template <typename S>
  self_type& operator/=(S const& scalar) {
    assert(scalar != static_cast<S>(0));
    for (int i = 0; i < size; ++i) {
      _arr[i] /= scalar;
    }
    return *this;
  }

  self_type& operator*=(self_type const& r) {
    self_type res;
    for (size_t i = 0; i < Rows; ++i) {
      for (size_t j = 0; j < Cols; ++j) {
        for (size_t step = 0; step < Cols; ++step) {
          res(i, j) += (*this)(i, step) * r(step, j);
        }
      }
    }
    std::memcpy(this->_arr, res._arr, sizeof(this->_arr));
    return *this;
  }

  template <int OtherCols>
  Mat<T, Rows, OtherCols> operator*(Mat<T, Cols, OtherCols> const& r) const {
    Mat<T, Rows, OtherCols> res;
    for (size_t i = 0; i < Rows; ++i) {
      for (size_t j = 0; j < OtherCols; ++j) {
        for (size_t step = 0; step < Cols; ++step) {
          res(i, j) += (*this)(i, step) * r(step, j);
        }
      }
    }
    return res;
  }

  T const* ptr() const { return &_arr[0]; }

  bool operator==(self_type const& r) const {
    return std::memcmp(this->_arr, r._arr, sizeof(this->_arr)) == 0;
  }

  bool operator!=(self_type const& r) const { return !(*this == r); }

  T dot(self_type const& r) const {
    T res(0);
    for (int i = 0; i < size; ++i) {
      res += _arr[i] * r[i];
    }
    return res;
  }

  T normsqr() const { return this->dot(*this); }

  T norm() const {
    using std::sqrt;
    return sqrt(this->normsqr());
  }

  self_type normalize() const {
    T invsqrt = static_cast<T>(1) / this->norm();
    return (*this) * invsqrt;
  }

  transpose_type transpose() const {
    transpose_type r;
    for (size_t i = 0; i < Rows; ++i) {
      for (size_t j = 0; j < Cols; ++j) {
        r(j, i) = (*this)(i, j);
      }
    }
    return r;
  }

  // MAP function style
  template <typename Func>
  self_type apply(Func foo) const {
    self_type r;
    std::transform(_arr, _arr + size, r._arr, foo);
    return r;
  }

  template <typename Func>
  self_type apply(self_type const& other, Func foo) const {
    self_type r;
    std::transform(_arr, _arr + size, other._arr, r._arr, foo);
    return r;
  }

  template <typename Func>
  void forEach(Func foo) {
    for (auto& e : (*this)) {
      foo(e);
    }
  }

  template <typename Func>
  void forEach(Func foo) const {
    for (auto const& e : (*this)) {
      foo(e);
    }
  }

  template <typename Func>
  bool verify(Func foo) const {
    for (auto const& el : *this) {
      if (!foo(el)) {
        return false;
      }
    }
    return true;
  }

  // VECTORS STUFF
  template <int ORows, int OCols, VARIADIC_ARG>
  Mat(Mat<value_type, ORows, OCols> const& smaller, Args... args) : Mat() {
    static_assert(is_vector, "operation reserved to vectors");
    static_assert(smaller.is_vector, "operation reserved to vectors");
    static_assert(smaller.size < size, "operation reserved to grow vectors");
    std::memcpy(this->_arr, smaller.begin(), sizeof(value_type) * smaller.size);
    setAt(smaller.size, args...);
  }

  T const& x() const {
    static_assert(rows == 1 || cols == 1, "operation reserved to vectors");
    return _arr[0];
  }
  T const& y() const {
    static_assert(size >= 2, "size is too small for this operation");
    static_assert(is_vector, "operation reserved to vectors");
    return _arr[1];
  }
  T const& z() const {
    static_assert(size >= 3, "size is too small for this operation");
    static_assert(is_vector, "operation reserved to vectors");
    return _arr[2];
  }
  T const& w() const {
    static_assert(size >= 4, "size is too small for this operation");
    static_assert(is_vector, "operation reserved to vectors");
    return _arr[3];
  }
  T& x() {
    static_assert(rows == 1 || cols == 1, "operation reserved to vectors");
    return _arr[0];
  }
  T& y() {
    static_assert(size >= 2, "size is too small for this operation");
    static_assert(is_vector, "operation reserved to vectors");
    return _arr[1];
  }
  T& z() {
    static_assert(size >= 3, "size is too small for this operation");
    static_assert(is_vector, "operation reserved to vectors");
    return _arr[2];
  }
  T& w() {
    static_assert(size >= 4, "size is too small for this operation");
    static_assert(is_vector, "operation reserved to vectors");
    return _arr[3];
  }

  // SQUARE MAT STUFF
  T tr() const {
    static_assert(is_square, "operation only defined for square matrix");
    T sum(0);
    for (std::size_t i = 0; i < rows; ++i) {
      sum += (*this)(i, i);
    }
    return sum;
  }

  T det() const;

  self_type inv() const;

  // ITERATOR STUFF
  const_iterator_type begin() const { return _arr; }
  iterator_type begin() { return _arr; }
  const_iterator_type end() const { return _arr + size; }
  iterator_type end() { return _arr + size; }

};  // namespace arty

namespace details {
MAT_TEMP
static inline T det2x2(MAT_TYPE const& m) {
  static_assert(m.is_square, "det is only defined for square matrix");
  static_assert(m.rows == 2, "det2x2 is fot Mat2x2");
  return m[0] * m[3] - m[1] * m[2];
}

MAT_TEMP
static inline T det3x3(MAT_TYPE const& m) {
  static_assert(m.is_square, "det is only defined for square matrix");
  static_assert(m.rows == 3, "det3x3 is fot Mat3x3");
  return m[0] * m[4] * m[8] + m[1] * m[5] * m[6] + m[2] * m[3] * m[7] -
         m[2] * m[4] * m[6] - m[1] * m[3] * m[8] - m[0] * m[5] * m[7];
}

MAT_TEMP
static inline T det4x4(MAT_TYPE const& mat) {
  static_assert(mat.is_square, "det is only defined for square matrix");
  static_assert(mat.rows == 4, "det4x4 is for Mat4x4");

  T const& a = mat(0, 0);
  T const& b = mat(0, 1);
  T const& c = mat(0, 2);
  T const& d = mat(0, 3);
  T const& e = mat(1, 0);
  T const& f = mat(1, 1);
  T const& g = mat(1, 2);
  T const& h = mat(1, 3);
  T const& i = mat(2, 0);
  T const& j = mat(2, 1);
  T const& k = mat(2, 2);
  T const& l = mat(2, 3);
  T const& m = mat(3, 0);
  T const& n = mat(3, 1);
  T const& o = mat(3, 2);
  T const& p = mat(3, 3);

  T f00 = k * p - l * o;
  T f01 = j * p - l * n;
  T f02 = j * o - k * n;
  T f09 = i * p - l * m;
  T f10 = i * o - k * m;
  T f15 = i * n - j * m;

  T c00 = f * f00 - g * f01 + h * f02;
  T c04 = -e * f00 + g * f09 - h * f10;
  T c08 = e * f01 - f * f09 + h * f15;
  T c12 = -e * f02 + f * f10 - g * f15;

  return a * c00 + b * c04 + c * c08 + d * c12;
}

MAT_TEMP
static inline MAT_TYPE inv2x2(MAT_TYPE const& m) {
  static_assert(m.is_square, "inv is only defined for square matrix");
  static_assert(m.rows == 2, "inv2x2 is fot Mat2x2");
  static_assert(std::is_floating_point<T>(), "inv is for floating types");

  return (MAT_TYPE() * m.tr() - m) * static_cast<T>(1) / m.det();
}

MAT_TEMP
static inline MAT_TYPE inv3x3(MAT_TYPE const& m) {
  static_assert(m.is_square, "inv is only defined for square matrix");
  static_assert(m.rows == 3, "inv3x3 is fot Mat3x3");
  static_assert(std::is_floating_point<T>(), "inv is for floating types");

  auto x0 = m.col(0);
  auto x1 = m.col(1);
  auto x2 = m.col(2);
  auto invDet = static_cast<T>(1) / m.det();
  return MAT_TYPE::fromRows(x1.cross(x2), x2.cross(x0), x0.cross(x1)) * invDet;
}

MAT_TEMP
static inline MAT_TYPE inv4x4(MAT_TYPE const& mat) {
  static_assert(mat.is_square, "inv is only defined for square matrix");
  static_assert(mat.rows == 4, "inv4x4 is fot Mat4x4");
  static_assert(std::is_floating_point<T>(), "inv is for floating types");

  T const& a = mat(0, 0);
  T const& b = mat(0, 1);
  T const& c = mat(0, 2);
  T const& d = mat(0, 3);
  T const& e = mat(1, 0);
  T const& f = mat(1, 1);
  T const& g = mat(1, 2);
  T const& h = mat(1, 3);
  T const& i = mat(2, 0);
  T const& j = mat(2, 1);
  T const& k = mat(2, 2);
  T const& l = mat(2, 3);
  T const& m = mat(3, 0);
  T const& n = mat(3, 1);
  T const& o = mat(3, 2);
  T const& p = mat(3, 3);

  T f00 = k * p - l * o;
  T f01 = j * p - l * n;
  T f02 = j * o - k * n;
  T f03 = g * p - h * o;
  T f04 = f * p - h * n;
  T f05 = f * o - g * n;
  T f06 = g * l - h * k;
  T f07 = f * l - h * j;
  T f08 = f * k - g * j;
  T f09 = i * p - l * m;
  T f10 = i * o - k * m;
  T f11 = e * p - h * m;
  T f12 = e * o - g * m;
  T f13 = e * l - h * i;
  T f14 = e * k - g * i;
  T f15 = i * n - j * m;
  T f16 = e * n - f * m;
  T f17 = e * j - f * i;

  T c00 = f * f00 - g * f01 + h * f02;
  T c01 = -b * f00 + c * f01 - d * f02;
  T c02 = b * f03 - c * f04 + d * f05;
  T c03 = -b * f06 + c * f07 - d * f08;
  T c04 = -e * f00 + g * f09 - h * f10;
  T c05 = a * f00 - c * f09 + d * f10;
  T c06 = -a * f03 + c * f11 - d * f12;
  T c07 = a * f06 - c * f13 + d * f14;
  T c08 = e * f01 - f * f09 + h * f15;
  T c09 = -a * f01 + b * f09 - d * f15;
  T c10 = a * f04 - b * f11 + d * f16;
  T c11 = -a * f07 + b * f13 - d * f17;
  T c12 = -e * f02 + f * f10 - g * f15;
  T c13 = a * f02 - b * f10 + c * f15;
  T c14 = -a * f05 + b * f12 - c * f16;
  T c15 = a * f08 - b * f14 + c * f17;

  T det = a * c00 + b * c04 + c * c08 + d * c12;
  assert(det != 0);

  MAT_TYPE inverse{c00, c01, c02, c03,  //
                   c04, c05, c06, c07,  //
                   c08, c09, c10, c11,  //
                   c12, c13, c14, c15};

  return inverse * static_cast<T>(1) / det;
}

}  // namespace details

MAT_TEMP
T MAT_TYPE::det() const {
  static_assert(is_square, "det is only defined for square matrix");
  static_assert(rows <= 4, "det is not implemented for this size");
  if constexpr (rows == 2) {
    return details::det2x2(*this);
  }
  if constexpr (rows == 3) {
    return details::det3x3(*this);
  }
  // if (rows == 4) {
  return details::det4x4(*this);
  //}
}

MAT_TEMP
MAT_TYPE MAT_TYPE::inv() const {
  static_assert(is_square, "inv is only defined for square matrix");
  static_assert(rows <= 4, "inv is not implemented for this size");
  if constexpr (rows == 2) {
    return details::inv2x2(*this);
  }
  if constexpr (rows == 3) {
    return details::inv3x3(*this);
  }
  // if (rows == 4) {
  return details::inv4x4(*this);
  //}
}

MAT_TEMP
inline const MAT_TYPE operator+(MAT_TYPE l, MAT_TYPE const& r) {
  l += r;
  return l;
}

MAT_TEMP
inline const MAT_TYPE operator-(MAT_TYPE l, MAT_TYPE const& r) {
  l -= r;
  return l;
}

MAT_TEMP
inline const MAT_TYPE operator*(MAT_TYPE l, T const& r) {
  l *= r;
  return l;
}

MAT_TEMP
inline const MAT_TYPE operator/(MAT_TYPE l, T const& r) {
  l /= r;
  return l;
}

MAT_TEMP
inline const MAT_TYPE operator*(MAT_TYPE l, MAT_TYPE const& r) {
  l *= r;
  return l;
}

/* You should avoid using that */
template <typename T>
using Mat1 = Mat<T, 1, 1>;
using Mat1f = Mat1<float>;

template <typename T>
using Mat2x2 = Mat<T, 2, 2>;
using Mat2x2f = Mat2x2<float>;

template <typename T>
using Mat3x3 = Mat<T, 3, 3>;
using Mat3x3f = Mat3x3<float>;

template <typename T>
using Mat4x4 = Mat<T, 4, 4>;
using Mat4x4f = Mat4x4<float>;

template <typename T, int Dim>
using Vec = Mat<T, Dim, 1>;

template <typename T>
using Vec2 = Vec<T, 2>;
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;

template <typename T>
using Vec3 = Vec<T, 3>;
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;

template <typename T>
using Vec4 = Vec<T, 4>;
using Vec4f = Vec4<float>;

template <typename T>
Vec3<T> inline cross(Vec3<T> const& l, Vec3<T> const& r) {
  return Vec3<T>{
      l[1] * r[2] - r[1] * l[2],  //
      l[2] * r[0] - r[2] * l[0],  //
      l[0] * r[1] - r[0] * l[1],  //
  };
}

template <typename T>
class Quat : public Vec4<T> {
 public:
  using Base = Vec4<T>;
  Quat() : Base({0.f, 0.f, 0.f, 1.f}) {}
  Quat(std::initializer_list<T> l) : Base(l) {}
  Quat(T p[Base::size]) : Base(p) {}
  template <class... Args>
  Quat(Args const&... args) : Base({args...}) {}

  Vec3<T> operator*(Vec3<T> const& v) {
    T t2 = Base::_arr[0] * Base::_arr[1];
    T t3 = Base::_arr[0] * Base::_arr[2];
    T t4 = Base::_arr[0] * Base::_arr[3];
    T t5 = -Base::_arr[1] * Base::_arr[1];
    T t6 = Base::_arr[1] * Base::_arr[2];
    T t7 = Base::_arr[1] * Base::_arr[3];
    T t8 = -Base::_arr[2] * Base::_arr[2];
    T t9 = Base::_arr[2] * Base::_arr[3];
    T t10 = -Base::_arr[3] * Base::_arr[3];
    T v1new =
        2 * ((t8 + t10) * v[0] + (t6 - t4) * v[1] + (t3 + t7) * v[2]) + v[0];
    T v2new =
        2 * ((t4 + t6) * v[0] + (t5 + t10) * v[1] + (t9 - t2) * v[2]) + v[1];
    T v3new =
        2 * ((t7 - t3) * v[0] + (t2 + t9) * v[1] + (t5 + t8) * v[2]) + v[2];
    return Vec3<T>{v1new, v2new, v3new};
  }

  Mat4x4f toMat4x4() const {
    Mat4x4f tf;
    float qx = this->x();
    float qy = this->y();
    float qz = this->z();
    float qw = this->w();
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
    tf(3, 3) = 1.f;
    return tf;
  }

  Mat3x3f toMat3x3() const {
    Mat3x3f tf;
    float qx = this->x();
    float qy = this->y();
    float qz = this->z();
    float qw = this->w();
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
    return tf;
  }

  void fromMat(Mat4x4f const& m) {
    this->w() = std::sqrt(1 + m(0, 0) + m(1, 1) + m(2, 2)) / 2;
    this->x() = (m(2, 1) - m(1, 2)) / (4 * this->w());
    this->y() = (m(0, 2) - m(2, 0)) / (4 * this->w());
    this->z() = (m(1, 0) - m(0, 1)) / (4 * this->w());
  }
};

using Quatf = Quat<float>;

template <typename T>
inline T radians(T const& degree) {
  return degree * static_cast<T>(0.01745329251994329576923690768489);
}

MAT_TEMP
static std::ostream& operator<<(std::ostream& out, MAT_TYPE const& mat) {
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
  Mat4x4<T> res = Mat4x4<T>::identity();
  res(0, 3) = x;
  res(1, 3) = y;
  res(2, 3) = z;
  return res;
}

template <typename T>
inline Mat4x4<T> translation(Vec3<T> const& t) {
  Mat4x4<T> res = Mat4x4<T>::identity();
  res(0, 3) = t.x();
  res(1, 3) = t.y();
  res(2, 3) = t.z();
  return res;
}

template <typename T>
inline Mat4x4<T> rotation(T const& a, T const& b, T const& c) {
  Mat4x4<T> res = Mat4x4<T>::identity();
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
