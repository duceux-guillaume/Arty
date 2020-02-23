#ifndef MACHINE_LEARNING_HPP
#define MACHINE_LEARNING_HPP

#include <any>
#include <arty/core/dynamic_matrix.hpp>
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <random>
#include <utility>

namespace arty {

static Matrix GenerateRandom(Matrix::size_t r, Matrix::size_t c,
                             Matrix::val_t min, Matrix::val_t max) {
  std::default_random_engine generator;
  std::uniform_real_distribution<Matrix::val_t> distribution(min, max);
  auto dice = std::bind(distribution, generator);
  Matrix res(r, c);
  for (auto& v : res) {
    v = dice();
  }
  return res;
}

class Block {
 public:
  using Ptr = std::shared_ptr<Block>;
  using array_type = Matrix;
  using value_type = Matrix::val_t;

  Block(Matrix::size_t in, Matrix::size_t ou) : _output(ou, 1), _input(in, 1) {}
  array_type const& output() const { return _output; }
  array_type const& input() const { return _input; }

  virtual array_type forward(array_type const& input) = 0;
  virtual array_type backward(array_type const& truth) = 0;
  virtual array_type gradient(array_type const& truth) = 0;

 protected:
  array_type _output;
  array_type _input;
};

class WeightBlock : public Block {
 public:
  WeightBlock(array_type const& p) : Block(p.cols(), p.rows()), _param(p) {}

  array_type forward(array_type const& input) override {
    _input = input;
    _output = _param * _input;
    return _output;
  }

  array_type backward(array_type const& truth) override {
    return inverse(_param) * truth;
  }

  array_type gradient(array_type const& /*error*/) override { return _param; }

 private:
  array_type inverse(array_type const& arr) {
    return array_type(arr.rows(), arr.cols());
  }

 private:
  array_type _param;
};

class FuncBlock : public Block {
 public:
  using func_type = std::function<array_type(array_type const&)>;
};

class Machine {
 public:
  using value_type = Block::value_type;
  using array_type = Block::array_type;

  Machine& addBlock(Block::Ptr block) {
    assert(block);
    _graph.push_back(block);
    return *this;
  }

  array_type forward(array_type const& input) {
    auto tmp = input;
    for (auto const& block : _graph) {
      tmp = block->forward(tmp);
    }
    return tmp;
  }

 private:
  std::vector<Block::Ptr> _graph;
};

class Learner {};

}  // namespace arty

#endif  // MACHINE_LEARNING_HPP
