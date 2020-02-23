#include <gtest/gtest.h>

#include <arty/core/machine_learning.hpp>

using namespace arty;

TEST(Machine, WeightBlock) {
  WeightBlock sum(GenerateRandom(2, 10, -1., 1.));
  auto output = sum.forward(GenerateRandom(10, 1, -1., 1.));
  ASSERT_EQ(output.size(), 2);
}
