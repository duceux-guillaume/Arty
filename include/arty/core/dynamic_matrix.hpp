#ifndef DYNAMIC_MATRIX_HPP
#define DYNAMIC_MATRIX_HPP

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <vector>

namespace arty {

class Matrix {
 public:
  using val_t = double;
  using arr_t = std::vector<val_t>;
  using size_t = std::size_t;
  using it_t = arr_t::iterator;
  using const_it_t = arr_t::const_iterator;

  Matrix(size_t rows, size_t cols)
      : _rows(rows), _cols(cols), _arr(_rows * _cols, val_t(0)) {}
  Matrix(size_t rows, size_t cols, val_t v)
      : _rows(rows), _cols(cols), _arr(_rows * _cols, v) {}
  Matrix(size_t rows, size_t cols, std::initializer_list<val_t> l)
      : _rows(rows), _cols(cols), _arr(l) {
    assert(l.size() == _rows * _cols);
  }

  // SPECIAL CONSTRUCTOR
  static Matrix diagonal(size_t s, val_t const& v);
  static Matrix identity(size_t s);

  size_t rows() const;
  size_t cols() const;
  size_t size() const;

  // Getters
  val_t const& at(size_t i, size_t j) const;
  val_t& at(size_t i, size_t j);
  val_t const& operator()(size_t i, size_t j) const;
  val_t& operator()(size_t i, size_t j);
  val_t const& operator[](size_t i) const;
  val_t& operator[](size_t i);

  // Operators
  Matrix& operator*=(val_t const& s);
  Matrix& operator+=(Matrix const& o);
  Matrix& operator-=(Matrix const& o);
  Matrix operator*(Matrix const& o) const;
  Matrix operator-() const;
  bool operator==(Matrix const& r) const;
  bool operator!=(Matrix const& r) const;

  // Iterators
  const_it_t begin() const;
  const_it_t end() const;
  it_t begin();
  it_t end();

  // Matrix
  val_t dot(Matrix const& r) const;
  val_t normsqr() const;
  val_t norm() const;
  Matrix transpose() const;

 private:
  size_t _rows;
  size_t _cols;
  arr_t _arr;
};

inline const Matrix operator*(Matrix l, Matrix::val_t const& r) {
  l *= r;
  return l;
}

inline const Matrix operator+(Matrix l, Matrix const& r) {
  l += r;
  return l;
}

inline const Matrix operator-(Matrix l, Matrix const& r) {
  l -= r;
  return l;
}

}  // namespace arty

[[maybe_unused]] std::ostream& operator<<(std::ostream& out,
                                          arty::Matrix const& mat);

#endif  // DYNAMIC_MATRIX_HPP
