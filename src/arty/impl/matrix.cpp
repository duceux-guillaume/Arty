#include <cassert>

#include <arty/impl/matrix.h>

namespace arty {

Matrix::Matrix(const size_t &row) : Matrix(row, 1) {}

Matrix::Matrix(const size_t &row, const size_t &col) : _dim(row, col) {
  _values.resize(_dim.cols() * _dim.rows());
}

Matrix::Matrix(const Dimension &dim) : Matrix(dim.rows(), dim.cols()) {}

const Number &Matrix::operator()(const size_t &row, const size_t &col) const {
  assert(row < _dim.rows());
  assert(col < _dim.cols());
  return _values[row * _dim.cols() + col];
}

Number &Matrix::operator()(const size_t &row, const size_t &col) {
  assert(row < _dim.rows());
  assert(col < _dim.cols());
  return _values[row * _dim.cols() + col];
}

Matrix &Matrix::operator*=(const Number &other) {
  const auto it_end = _values.end();
  for (auto it = _values.begin(); it != it_end; ++it) {
    *it *= other;
  }
  return *this;
}

Matrix &Matrix::operator+=(const Matrix &rhs) {
  assert(_dim == other._dim);
  auto this_it = _values.begin();
  const auto this_end = _values.end();
  auto other_it = rhs._values.begin();
  for (; this_it != this_end; ++this_it, ++other_it) {
    *this_it += *other_it;
  }
  return *this;
}

Matrix Matrix::operator*(const Matrix &other) const {
  assert(_dim.cols() == other._dim.rows());
  Matrix res(_dim.rows(), other._dim.cols());
  for (size_t i = 0; i < res._dim.rows(); ++i) {
    for (size_t j = 0; j < res._dim.cols(); ++j) {
      for (size_t step = 0; step < res._dim.rows(); ++step) {
        res(i, j) = res(i, j) + (*this)(i, step) * other(step, j);
      }
    }
  }
  return res;
}

Matrix Matrix::operator-(const Matrix &other) const {
  assert(_dim == other._dim);
  Matrix res(_dim);
  for (std::size_t i = 0; i < _dim.rows(); ++i) {
    for (std::size_t j = 0; j < _dim.cols(); ++j) {
      res(i, j) = (*this)(i, j) - other(i, j);
    }
  }
  return res;
}

Dimension::Dimension(const size_t &row) : Dimension(row, 1) {}

Dimension::Dimension(const size_t &row, const size_t &col)
    : _rows(row), _cols(col) {}

bool Dimension::operator==(const Dimension &other) const {
  return _rows == other._rows && _cols == other._cols;
}

std::ostream &operator<<(std::ostream &out, const Dimension &dim) {
  return out << '(' << dim.rows() << ", " << dim.cols() << ")";
}

std::ostream &operator<<(std::ostream &out, const Matrix &mat) {
  out << mat.dim() << std::endl;
  for (std::size_t i = 0; i < mat.dim().rows(); ++i) {
    out << "|";
    for (std::size_t j = 0; j < mat.dim().cols(); ++j) {
      out << mat(i, j) << " ";
    }
    out << "|\n";
  }
  return out;
}

}  // namespace arty
