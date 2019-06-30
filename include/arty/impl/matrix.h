#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

#include <arty/core/number.h>

namespace arty {

class Dimension {
 private:
  size_t _rows;
  size_t _cols;

 public:
  Dimension(size_t const& row);
  Dimension(size_t const& row, size_t const& col);

  size_t const& cols() const { return _cols; }
  size_t const& rows() const { return _rows; }

  bool operator==(Dimension const& other) const;
};

class Matrix {
 public:
  using number_t = Number;
  using storage_t = std::vector<Number>;

 private:
  Dimension _dim;
  storage_t _values;

 public:
  explicit Matrix(size_t const& row);
  Matrix(size_t const& row, size_t const& col);
  Matrix(Dimension const& dim);

  Number const& operator()(size_t const& row, size_t const& col) const;
  Number& operator()(size_t const& row, size_t const& col);

  Matrix& operator*=(Number const& other);
  Matrix& operator+=(Number const& other);

  Matrix& operator+=(Matrix const& rhs);

  Matrix operator*(Matrix const& other) const;
  Matrix operator-(Matrix const& other) const;

  Dimension const& dim() const { return _dim; }

  storage_t const& values() const { return _values; }
  storage_t& values() { return _values; }

  Matrix transposed() const;
};

inline const Matrix operator*(Matrix l, Number const& r) {
  l *= r;
  return l;
}
inline const Matrix operator*(Number const& l, Matrix r) {
  r *= l;
  return r;
}
inline const Matrix operator+(Matrix l, Matrix const& r) {
  l += r;
  return l;
}

std::ostream& operator<<(std::ostream& out, Dimension const& dim);
std::ostream& operator<<(std::ostream& out, Matrix const& mat);

}  // namespace arty

#endif  // MATRIX_H
