#include <arty/core/machine_learning.hpp>
#include <chrono>

namespace arty {

Block::array_type WeightBlock::forward(const Block::array_type& input) {
  _input = input;
  if (_input.cols() > 1) {
    _input = _input.flatten();
  }
  _output = _params * _input;
  return _output;
}

Block::array_type WeightBlock::backward(const Block::array_type& error) {
  return _params.transpose() * error;
}

Block::array_type WeightBlock::gradient(const Block::array_type& error) {
  return error * _input.transpose() * (-1);
}

Matrix Block::RandomMatrix(Matrix::size_t r, Matrix::size_t c,
                           Matrix::val_t min, Matrix::val_t max) {
  unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<Matrix::val_t> distribution(min, max);
  auto dice = std::bind(distribution, generator);
  Matrix res(r, c);
  for (auto& v : res) {
    v = dice();
  }
  return res;
}

Block::array_type FuncBlock::forward(const Block::array_type& input) {
  _input = input;
  _output = _func(_input + _params);
  return _output;
}

Block::array_type FuncBlock::backward(const Block::array_type& error) {
  return _inve(error);
}

Block::array_type FuncBlock::gradient(const Block::array_type& error) {
  return -error;
}

}  // namespace arty
