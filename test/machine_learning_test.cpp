#include <gtest/gtest.h>

#include <arty/core/machine_learning.hpp>

using namespace arty;

TEST(WeightBlock, LearningWeightBlock) {
  WeightBlock sum(Block::RandomMatrix(2, 10, -1., 1.));
  auto input = Block::RandomMatrix(10, 1, -1., 1.);
  Matrix truth(2, 1, {1, 0});
  //  std::cout << "params" << sum.params();
  //  std::cout << "input" << input;
  //  std::cout << "truth" << truth;
  double rate = 0.1;
  double threshold = 1e-10;
  double score = 1e3;
  int iteration = 0;
  double last_score = score;

  // Init Error
  auto foutput = sum.forward(input);
  Matrix first_error = truth - foutput;
  //  std::cout << "first ouput" << foutput;
  //  std::cout << "first error" << first_error << first_error.norm() <<
  //  std::endl;

  do {
    last_score = score;
    ++iteration;

    // Forward & Error
    auto output = sum.forward(input);
    Matrix error = truth - output;
    score = error.norm();
    //    std::cout << "ouput" << output;
    //    std::cout << "error" << error << score << std::endl;
    ASSERT_LE(score, first_error.norm());
    ASSERT_LE(score, last_score);

    // Learn
    Matrix gradient = sum.gradient(error);
    //    std::cout << "params" << sum.params();
    //    std::cout << "gradient" << gradient;
    sum.setParams(sum.params() - gradient * rate);

    //    std::cout << "score: " << score << " iterations: " << iteration
    //              << " last: " << last_score << std::endl;
  } while (score > threshold && last_score != score);
  std::cout << "score: " << score << " iterations: " << iteration
            << " last: " << last_score << std::endl;
}

TEST(WeightBlock, BackwardWeightBlock) {
  WeightBlock sum(Block::RandomMatrix(2, 10, -1., 1.));
  auto input = Block::RandomMatrix(10, 1, -1., 1.);
  Matrix truth(2, 1, {1, 0});
  // Forward
  auto output = sum.forward(input);
  Matrix error = truth - output;
  auto score = error.norm();
  // std::cout << "input" << input;
  // std::cout << "ouput" << output;
  // std::cout << "error" << error << score << std::endl;
  auto score2 = score;
  int iteration = 0;
  do {
    ++iteration;
    // Backward
    auto backward = sum.backward(truth - sum.output());
    //    std::cout << "backward" << backward;
    auto output2 = sum.forward(sum.input() + backward * 0.2);
    Matrix error2 = truth - output2;
    score2 = error2.norm();
    //    std::cout << "ouput" << output2;
    //    std::cout << "error" << error2 << score2 << std::endl;
    ASSERT_LT(score2, score);
    //    std::cout << "score2: " << score2 << " iterations: " << iteration
    //              << std::endl;
  } while (score2 > 0.01);
  std::cout << "final" << sum.forward(sum.input());
}

TEST(FuncBlock, LearningFuncBlock) {
  auto relu = [](Block::array_type const& m) -> Block::array_type {
    Block::array_type result(m);
    std::for_each(result.begin(), result.end(), [](Block::value_type& v) {
      v = std::max(Block::value_type(0), v);
    });
    return result;
  };
  auto inv = [](Block::array_type const& m) -> Block::array_type { return m; };
  auto der = [](Block::array_type const& m) -> Block::array_type {
    Block::array_type result(m);
    std::for_each(result.begin(), result.end(),
                  [](Block::value_type& v) { v = (v > 0) ? 1 : 0; });
    return result;
  };

  FuncBlock sum(Block::RandomMatrix(3, 1, -1., 1.), relu, inv, der);
  auto input = Block::RandomMatrix(3, 1, -1., 1.);
  Matrix truth(3, 1, {1, 0, 0});
  //  std::cout << "params" << sum.params();
  //  std::cout << "input" << input;
  //  std::cout << "truth" << truth;
  double rate = 0.1;
  double threshold = 1e-10;
  double score = 1e3;
  int iteration = 0;
  double last_score = score;

  // Init Error
  auto foutput = sum.forward(input);
  Matrix first_error = truth - foutput;
  //  std::cout << "first ouput" << foutput;
  //  std::cout << "first error" << first_error << first_error.norm() <<
  //  std::endl;

  do {
    last_score = score;
    ++iteration;

    // Forward & Error
    auto output = sum.forward(input);
    Matrix error = truth - output;
    score = error.norm();
    //    std::cout << "ouput" << output;
    //    std::cout << "error" << error << score << std::endl;
    ASSERT_LE(score, first_error.norm());
    ASSERT_LE(score, last_score);

    // Learn
    Matrix gradient = sum.gradient(error);
    //    std::cout << "params" << sum.params();
    //    std::cout << "gradient" << gradient;
    sum.setParams(sum.params() - gradient * rate);

    //    std::cout << "score: " << score << " iterations: " << iteration
    //              << " last: " << last_score << std::endl;
  } while (score > threshold && last_score != score);
  std::cout << "score: " << score << " iterations: " << iteration
            << " last: " << last_score << std::endl;
}

TEST(FuncBlock, BackwardFuncBlock) {
  auto relu = [](Block::array_type const& m) -> Block::array_type {
    Block::array_type result(m);
    std::for_each(result.begin(), result.end(), [](Block::value_type& v) {
      v = std::max(Block::value_type(0), v);
    });
    return result;
  };
  auto inv = [](Block::array_type const& m) -> Block::array_type { return m; };
  auto der = [](Block::array_type const& m) -> Block::array_type {
    Block::array_type result(m);
    std::for_each(result.begin(), result.end(),
                  [](Block::value_type& v) { v = (v > 0) ? 1 : 0; });
    return result;
  };

  FuncBlock sum(Block::RandomMatrix(3, 1, -1., 1.), relu, inv, der);
  auto input = Block::RandomMatrix(3, 1, -1., 1.);
  Matrix truth(3, 1, {1, 0, 0});
  // Forward
  auto output = sum.forward(input);
  Matrix error = truth - output;
  auto score = error.norm();
  std::cout << "params" << sum.params();
  std::cout << "input" << input;
  std::cout << "ouput" << output;
  std::cout << "error" << error << score << std::endl;
  auto score2 = score;
  int iteration = 0;
  do {
    ++iteration;
    // Backward
    auto backward = sum.backward(truth - sum.output());
    std::cout << "backward" << backward;
    auto output2 = sum.forward(sum.input() + backward * 0.2);
    Matrix error2 = truth - output2;
    score2 = error2.norm();
    std::cout << "ouput" << output2;
    std::cout << "error" << error2 << score2 << std::endl;
    ASSERT_LT(score2, score);
    std::cout << "score2: " << score2 << " iterations: " << iteration
              << std::endl;
  } while (score2 > 0.01);
  std::cout << "final" << sum.forward(sum.input());
}

TEST(Learner, VanillaNet) {
  Matrix zero(8, 8,
              {
                  0, 0, 0, 0, 0, 0, 0, 0,  //
                  0, 0, 0, 1, 1, 0, 0, 0,  //
                  0, 0, 1, 0, 0, 1, 0, 0,  //
                  0, 0, 1, 0, 0, 1, 0, 0,  //
                  0, 0, 1, 0, 0, 1, 0, 0,  //
                  0, 0, 1, 0, 0, 1, 0, 0,  //
                  0, 0, 0, 1, 1, 0, 0, 0,  //
                  0, 0, 0, 0, 0, 0, 0, 0,  //
              });

  Matrix one(8, 8,
             {
                 0, 0, 0, 0, 0, 0, 0, 0,  //
                 0, 0, 0, 0, 1, 0, 0, 0,  //
                 0, 0, 0, 1, 1, 0, 0, 0,  //
                 0, 0, 0, 0, 1, 0, 0, 0,  //
                 0, 0, 0, 0, 1, 0, 0, 0,  //
                 0, 0, 0, 0, 1, 0, 0, 0,  //
                 0, 0, 0, 0, 1, 0, 0, 0,  //
                 0, 0, 0, 0, 0, 0, 0, 0,  //
             });

  Matrix two(8, 8,
             {
                 0, 0, 0, 0, 0, 0, 0, 0,  //
                 0, 0, 0, 1, 1, 0, 0, 0,  //
                 0, 0, 1, 0, 0, 1, 0, 0,  //
                 0, 0, 0, 0, 1, 0, 0, 0,  //
                 0, 0, 0, 1, 0, 0, 0, 0,  //
                 0, 0, 1, 0, 0, 0, 0, 0,  //
                 0, 0, 1, 1, 1, 1, 0, 0,  //
                 0, 0, 0, 0, 0, 0, 0, 0,  //
             });

  Matrix three(8, 8,
               {
                   0, 0, 0, 0, 0, 0, 0, 0,  //
                   0, 0, 0, 1, 1, 0, 0, 0,  //
                   0, 0, 1, 0, 0, 1, 0, 0,  //
                   0, 0, 0, 0, 1, 0, 0, 0,  //
                   0, 0, 0, 0, 1, 0, 0, 0,  //
                   0, 0, 1, 0, 0, 1, 0, 0,  //
                   0, 0, 0, 1, 1, 0, 0, 0,  //
                   0, 0, 0, 0, 0, 0, 0, 0,  //
               });

  Machine machine;
  Block::Ptr sum(new WeightBlock(Block::RandomMatrix(4, 64, -0.5, 0.5)));
  machine.addBlock(sum);
  auto relu = [](Block::array_type const& m) -> Block::array_type {
    Block::array_type result(m);
    std::for_each(result.begin(), result.end(), [](Block::value_type& v) {
      v = std::max(Block::value_type(0), v);
    });
    return result;
  };
  auto inv = [](Block::array_type const& m) -> Block::array_type { return m; };
  auto der = [](Block::array_type const& m) -> Block::array_type {
    Block::array_type result(m);
    std::for_each(result.begin(), result.end(),
                  [](Block::value_type& v) { v = (v > 0) ? 1 : 0; });
    return result;
  };
  Block::Ptr fun(
      new FuncBlock(Block::RandomMatrix(4, 1, -1., 1.), relu, inv, der));
  machine.addBlock(fun);

  Learner learner;
  learner.machine(machine).learningRate(0.1);

  std::vector<Matrix> dataset{zero, one, two, three};
  std::vector<Matrix> label{
      Matrix(4, 1, {1, 0, 0, 0}), Matrix(4, 1, {0, 1, 0, 0}),
      Matrix(4, 1, {0, 0, 1, 0}), Matrix(4, 1, {0, 0, 0, 1})};

  std::size_t iteration = 0;
  std::cout << "start 0" << learner.forward(zero);
  std::cout << "error 0" << learner.test(zero, label[0]).norm() << std::endl;
  std::cout << "start 1" << learner.forward(one);
  std::cout << "error 1" << learner.test(one, label[1]).norm() << std::endl;
  std::cout << "start 2" << learner.forward(two);
  std::cout << "error 2" << learner.test(two, label[2]).norm() << std::endl;
  std::cout << "start 3" << learner.forward(three);
  std::cout << "error 3" << learner.test(three, label[3]).norm() << std::endl;
  do {
    learner.train(dataset[iteration % 4], label[iteration % 4]);
    learner.test(dataset[iteration % 4], label[iteration % 4]);
  } while (++iteration < 100);
  std::cout << "0" << learner.forward(zero);
  std::cout << "error 0" << learner.test(zero, label[0]).norm() << std::endl;
  std::cout << "1" << learner.forward(one);
  std::cout << "error 1" << learner.test(one, label[1]).norm() << std::endl;
  std::cout << "2" << learner.forward(two);
  std::cout << "error 2" << learner.test(two, label[2]).norm() << std::endl;
  std::cout << "3" << learner.forward(three);
  std::cout << "error 3" << learner.test(three, label[3]).norm() << std::endl;
}
