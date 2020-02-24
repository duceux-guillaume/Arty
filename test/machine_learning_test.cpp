#include <gtest/gtest.h>

#include <arty/core/machine_learning.hpp>

using namespace arty;

TEST(Machine, LearningWeightBlock) {
  WeightBlock sum(WeightBlock::RandomMatrix(2, 10, -1., 1.));
  auto input = WeightBlock::RandomMatrix(10, 1, -1., 1.);
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

TEST(Machine, BackwardWeightBlock) {
  WeightBlock sum(WeightBlock::RandomMatrix(2, 10, -1., 1.));
  auto input = WeightBlock::RandomMatrix(10, 1, -1., 1.);
  Matrix truth(2, 1, {1, 0});
  // Forward
  auto output = sum.forward(input);
  Matrix error = truth - output;
  auto score = error.norm();
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
