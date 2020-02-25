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

class Block {
 public:
  using Ptr = std::shared_ptr<Block>;
  using array_type = Matrix;
  using value_type = Matrix::val_t;

  Block(Matrix::size_t in, Matrix::size_t ou, array_type const& p)
      : _output(ou, 1), _input(in, 1), _params(p) {}

  array_type const& output() const { return _output; }
  array_type const& input() const { return _input; }
  array_type const& params() const { return _params; };
  void setParams(array_type const& p) { _params = p; }

  virtual array_type forward(array_type const& input) = 0;
  virtual array_type backward(array_type const& truth) = 0;
  virtual array_type gradient(array_type const& truth) = 0;

  static Matrix RandomMatrix(Matrix::size_t r, Matrix::size_t c,
                             Matrix::val_t min, Matrix::val_t max);

 protected:
  array_type _output;
  array_type _input;
  array_type _params;
};

class WeightBlock : public Block {
 public:
  WeightBlock(array_type const& weights)
      : Block(weights.cols(), weights.rows(), weights) {}

  array_type forward(array_type const& input) override;

  array_type backward(array_type const& error) override;

  array_type gradient(array_type const& error) override;
};

class FuncBlock : public Block {
 public:
  using func_type = std::function<array_type(array_type const&)>;

  FuncBlock(array_type const& bias, func_type f, func_type i, func_type d)
      : Block(bias.rows(), bias.rows(), bias), _func(f), _inve(i), _deri(d) {
    assert(bias.cols() == 1);
  }

  array_type forward(array_type const& input) override;

  array_type backward(array_type const& error) override;

  array_type gradient(array_type const& error) override;

 private:
  func_type _func;
  func_type _inve;
  func_type _deri;
};

class Machine {
 public:
  using value_type = Block::value_type;
  using array_type = Block::array_type;
  using storage_type = std::vector<Block::Ptr>;
  using rev_it_type = storage_type::reverse_iterator;

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

  rev_it_type rbegin() { return _graph.rbegin(); }
  rev_it_type rend() { return _graph.rend(); }

 private:
  std::vector<Block::Ptr> _graph;
};

class Learner {
 public:
  Learner& machine(Machine const& m) {
    _machine = m;
    return *this;
  }
  Learner& learningRate(double r) {
    _learning_rate = r;
    return *this;
  }

  void train(Block::array_type const& example, Block::array_type const& label) {
    auto error = test(example, label);
    // std::cout << "error: " << error << error.norm() << std::endl;
    std::for_each(
        _machine.rbegin(), _machine.rend(), [this, &error](Block::Ptr& layer) {
          // std::cout << "params:" << layer->params();
          auto gradient = layer->gradient(error);
          error = layer->backward(error);
          layer->setParams(layer->params() - gradient * _learning_rate);
        });
  }

  Block::array_type test(Block::array_type const& example,
                         Block::array_type const& label) {
    auto error = label - _machine.forward(example);
    return error;
  }

  Block::array_type forward(Block::array_type const& input) {
    return _machine.forward(input);
  }

 private:
  Machine _machine;
  double _learning_rate;
};

}  // namespace arty

#endif  // MACHINE_LEARNING_HPP
