#ifndef LEARNING_H
#define LEARNING_H

#include <arty/core/number.h>
#include <arty/impl/matrix.h>

namespace arty {

Matrix char_to_matrix(char c);
char matrix_to_char(Matrix const& mat);

class Perceptron {
 public:
  Perceptron(size_t const& input_size, size_t const& output_size);

  Matrix predict(Matrix const& input);
  void learn(Matrix const& input, Matrix const& target);

  Matrix const& weights() const { return _weights; }
  Matrix const& bias() const { return _bias; }

 private:
  Number _lrate;
  Matrix _bias;
  Matrix _weights;
};

}  // namespace arty

#endif  // LEARNING_H
