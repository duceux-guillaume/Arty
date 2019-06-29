#include <arty/impl/learning.h>

#include <gtest/gtest.h>

using arty::Matrix;
using arty::Perceptron;

TEST(Func, Char2Matrix) {
  Matrix c = arty::char_to_matrix('c');
  std::cout << c << std::endl;
}

TEST(Perceptron, Constructor) { Perceptron(255, 255); }

TEST(Perceptron, Predict) {
  Perceptron p(255, 255);
  Matrix r = p.predict(arty::char_to_matrix('l'));
  std::cout << r << std::endl;
  std::cout << matrix_to_char(r) << std::endl;
}

TEST(Perceptron, Learn) {
  Perceptron p(255, 255);
  Matrix l = arty::char_to_matrix('l');
  for (int i = 48; i < 59; ++i) {
    p.learn(arty::char_to_matrix(char(i)), arty::char_to_matrix(char(i)));
    Matrix prediction = p.predict(arty::char_to_matrix(char(i)));
    std::cout << "error: " << arty::matrix_to_char(prediction) << " - "
              << char(i) << std::endl;
  }
  std::cout << p.weights() << p.bias();
}
