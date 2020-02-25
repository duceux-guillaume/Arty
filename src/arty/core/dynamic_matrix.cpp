#include <algorithm>
#include <arty/core/dynamic_matrix.hpp>
#include <cmath>

namespace arty {

Matrix Matrix::diagonal(Matrix::size_t s, const Matrix::val_t& v) {
  Matrix m(s, s);
  for (size_t i = 0; i < m._rows; ++i) {
    m(i, i) = v;
  }
  return m;
}

Matrix Matrix::identity(Matrix::size_t s) { return diagonal(s, val_t(1)); }

Matrix::size_t Matrix::rows() const { return _rows; }

Matrix::size_t Matrix::cols() const { return _cols; }

Matrix::size_t Matrix::size() const { return _arr.size(); }

const Matrix::val_t& Matrix::at(Matrix::size_t i, Matrix::size_t j) const {
  assert(i < _rows);
  assert(j < _cols);
  return _arr[i * _cols + j];
}

Matrix::val_t& Matrix::at(Matrix::size_t i, Matrix::size_t j) {
  assert(i < _rows);
  assert(j < _cols);
  return _arr[i * _cols + j];
}

const Matrix::val_t& Matrix::operator()(Matrix::size_t i,
                                        Matrix::size_t j) const {
  assert(i < _rows);
  assert(j < _cols);
  return _arr[i * _cols + j];
}

Matrix::val_t& Matrix::operator()(Matrix::size_t i, Matrix::size_t j) {
  assert(i < _rows);
  assert(j < _cols);
  return _arr[i * _cols + j];
}

const Matrix::val_t& Matrix::operator[](Matrix::size_t i) const {
  assert(i < size());
  return _arr[i];
}

Matrix::val_t& Matrix::operator[](Matrix::size_t i) {
  assert(i < size());
  return _arr[i];
}

Matrix& Matrix::operator*=(const Matrix::val_t& s) {
  std::for_each(_arr.begin(), _arr.end(), [&s](val_t& n) { n *= s; });
  return *this;
}

Matrix& Matrix::operator+=(const Matrix& o) {
  assert(_arr.size() == o._arr.size());
  std::transform(o._arr.begin(), o._arr.end(), _arr.begin(), _arr.begin(),
                 [](val_t const& a, val_t const& b) -> val_t { return a + b; });
  return *this;
}

Matrix& Matrix::operator-=(const Matrix& o) {
  assert(_arr.size() == o._arr.size());
  std::transform(_arr.begin(), _arr.end(), o._arr.begin(), _arr.begin(),
                 [](val_t const& a, val_t const& b) -> val_t { return a - b; });
  return *this;
}

Matrix Matrix::operator*(const Matrix& o) const {
  assert(o.rows() == _cols);
  Matrix res(_rows, o._cols);
  for (size_t i = 0; i < _rows; ++i) {
    for (size_t j = 0; j < o._cols; ++j) {
      for (size_t step = 0; step < _cols; ++step) {
        res(i, j) += (*this)(i, step) * o(step, j);
      }
    }
  }
  return res;
}

Matrix Matrix::operator-() const {
  Matrix r(*this);
  std::for_each(r.begin(), r.end(), [](val_t& v) { v = -v; });
  return r;
}

bool Matrix::operator==(const Matrix& r) const { return _arr == r._arr; }

bool Matrix::operator!=(const Matrix& r) const { return !(*this == r); }

Matrix::const_it_t Matrix::begin() const { return _arr.begin(); }

Matrix::const_it_t Matrix::end() const { return _arr.end(); }

Matrix::it_t Matrix::begin() { return _arr.begin(); }

Matrix::it_t Matrix::end() { return _arr.end(); }

Matrix::val_t Matrix::dot(const Matrix& r) const {
  assert(_arr.size() == r._arr.size());
  val_t res(0);
  for (size_t i = 0; i < _arr.size(); ++i) {
    res += _arr[i] * r[i];
  }
  return res;
}

Matrix::val_t Matrix::normsqr() const { return dot(*this); }

Matrix::val_t Matrix::norm() const {
  using std::sqrt;
  return sqrt(this->normsqr());
}

Matrix Matrix::transpose() const {
  Matrix r(_cols, _rows);
  for (size_t i = 0; i < _rows; ++i) {
    for (size_t j = 0; j < _cols; ++j) {
      r(j, i) = at(i, j);
    }
  }
  return r;
}

}  // namespace arty

std::ostream& operator<<(std::ostream& out, const arty::Matrix& mat) {
  out << "\n";
  for (std::size_t i = 0; i < mat.rows(); ++i) {
    out << "|";
    for (std::size_t j = 0; j < mat.cols(); ++j) {
      out << " " << mat(i, j);
    }
    out << "|\n";
  }
  return out;
}
