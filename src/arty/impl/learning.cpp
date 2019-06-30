#include <arty/impl/learning.h>

#include <cassert>

namespace arty {

Perceptron::Perceptron(const size_t &input_size, const size_t &output_size)
    : _lrate(1, 10), _bias(input_size), _weights(input_size, output_size) {}

Matrix Perceptron::predict(const Matrix &input) {
  Matrix h = _weights * input + _bias;
  for (auto &number : h.values()) {
    number = (number > 0) ? 1 : 0;
  }
  return h;
}

void Perceptron::learn(const Matrix &input, const Matrix &target) {
  Matrix prediction = predict(input);
  Matrix error = target - prediction;
  assert(error.dim().cols() == 1);
  assert(error.dim().rows() > 0);
  for (size_t i = 0; i < _weights.dim().rows(); ++i) {
    for (size_t j = 0; j < _weights.dim().cols(); ++j) {
      _weights(i, j) = _weights(i, j) + error(j, 0) * input(i, 0) * _lrate;
    }
  }
  _bias += error * _lrate;
}

Matrix char_to_matrix(char c) {
  Matrix res(255);
  res(size_t(c), 0) = 1;
  return res;
}

char matrix_to_char(const Matrix &mat) {
  assert(mat.dim().rows() == 255);
  assert(mat.dim().cols() == 1);
  Number max = mat(0, 0);
  size_t arg_max = 0;
  for (size_t i = 1; i < 255; ++i) {
    if (mat(i, 0) > max) {
      max = mat(i, 0);
      arg_max = i;
    }
  }
  return char(arg_max);
}

}  // namespace arty
