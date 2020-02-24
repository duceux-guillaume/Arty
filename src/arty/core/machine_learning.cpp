#include <arty/core/machine_learning.hpp>
#include <chrono>

namespace arty {

Block::array_type WeightBlock::forward(const Block::array_type& input) {
  _input = input;
  _output = _param * _input;
  return _output;
}

Block::array_type WeightBlock::gradient(const Block::array_type& error) {
  return error * _input.transpose() * (-1);
}

Matrix WeightBlock::RandomMatrix(Matrix::size_t r, Matrix::size_t c,
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

}  // namespace arty
