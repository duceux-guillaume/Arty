#include <gtest/gtest.h>

#include <arty/core/number.hpp>

using namespace arty;

TEST(Number, Constructor) {
  ASSERT_EQ(number(), 0);
  ASSERT_EQ(number(), number(0));
  ASSERT_EQ(number(), number(0, 1));
  ASSERT_EQ(number(), number(0.));
  ASSERT_EQ(number(), number(0.f));
  ASSERT_EQ(number(1), 1);
  ASSERT_EQ(number(1), number(1));
  ASSERT_EQ(number(1), number(1, 1));
  ASSERT_EQ(number(1), number(1.));
  ASSERT_EQ(number(1), number(1.f));
  ASSERT_EQ(number(0.1), 0.1);
  ASSERT_EQ(number(0.1), number(1, 10));
  ASSERT_EQ(number(0.1), number(0.1));
  ASSERT_EQ(number(0.1), number(0.1f));
}

TEST(Number, CastToDouble) {
  ASSERT_EQ(static_cast<double>(number(1000)), 1000.);
  ASSERT_EQ(static_cast<double>(number(0.1)), 0.1);
  ASSERT_EQ(static_cast<double>(number(1, 1000)), 0.001);
}

TEST(Number, CastToString) {
  ASSERT_EQ(static_cast<std::string>(number(0.1)), "1/10");
  ASSERT_EQ(static_cast<std::string>(number(1)), "1");
  ASSERT_EQ(static_cast<std::string>(number(5, 5)), "1");
}

TEST(Number, Equals) {
  ASSERT_TRUE(number() == number());
  ASSERT_FALSE(number() == number(1));
}

TEST(Number, Different) {
  ASSERT_FALSE(number() != number());
  ASSERT_TRUE(number() != number(1));
}

TEST(Number, LessThan) {
  ASSERT_TRUE(number() < number(1));
  ASSERT_TRUE(number(-1) < number(0));
  ASSERT_FALSE(number(1) < number(0));
}

TEST(Number, LessOrEquals) {
  ASSERT_TRUE(number() <= number(1));
  ASSERT_TRUE(number(1) <= number(1));
  ASSERT_FALSE(number(1) <= number(0));
}

TEST(Number, GreaterThan) {
  ASSERT_FALSE(number() > number(1));
  ASSERT_FALSE(number(0) > number(0));
  ASSERT_TRUE(number(1) > number(0));
}

TEST(Number, GreaterOrEquals) {
  ASSERT_FALSE(number() >= number(1));
  ASSERT_TRUE(number(1) >= number(1));
  ASSERT_TRUE(number(1) >= number(0));
}

TEST(Number, Neg) {
  ASSERT_EQ(-number(), number());
  ASSERT_EQ(-number(1), number(-1));
  ASSERT_NE(-number(1), number(1));
}

TEST(Number, Plus) {
  ASSERT_EQ(number() + number(), number());
  ASSERT_EQ(number(1) + number(2), number(3));
  ASSERT_EQ(number(2) + number(1), number(3));
  ASSERT_EQ(number(1, 2) + number(1, 3), number(5, 6));
}

TEST(Number, Minus) {
  ASSERT_EQ(number() - number(), number());
  ASSERT_EQ(number(1) - number(2), number(-1));
  ASSERT_EQ(number(2) - number(1), number(1));
  ASSERT_EQ(number(1, 2) - number(1, 3), number(1, 6));
}

TEST(Number, Times) {
  ASSERT_EQ(number() * number(), number());
  ASSERT_EQ(number(1) * number(2), number(2));
  ASSERT_EQ(number(2) * number(1), number(2));
  ASSERT_EQ(number(1, 2) * number(1, 3), number(1, 6));
}

TEST(Number, Over) {
  ASSERT_EQ(number() / number(15030), number());
  ASSERT_EQ(number(1) / number(2), number(1, 2));
  ASSERT_EQ(number(2) / number(1), number(2));
  ASSERT_EQ(number(1, 2) / number(1, 3), number(3, 2));
}

TEST(Number, Sqrt) {
  ASSERT_EQ(number::sqrt(1, 1), 1);
  ASSERT_EQ(number::sqrt(4, 1), 2);
  ASSERT_EQ(number::sqrt(9, 1), 3);
  ASSERT_EQ(number::sqrt(16, 1), 4);
  // sqrt(2) ~= 1.41421356237
  ASSERT_EQ(number::sqrt(2, 1), number(14, 10));
  // ASSERT_EQ(number::sqrt(2, 2), number(141, 10));
  // ASSERT_EQ(number::sqrt(2, 4), number(14142, 10000));
}

TEST(Number, PowOfIntegral) {
  ASSERT_EQ(number::pow(number(), 0, 1), 1);
  ASSERT_EQ(number::pow(number(1), 0, 1), 1);
  ASSERT_EQ(number::pow(number(1), 2, 1), 1);
  ASSERT_EQ(number::pow(number(2), 0, 1), 1);
  ASSERT_EQ(number::pow(number(2), 1, 1), 2);
  ASSERT_EQ(number::pow(number(2), 2, 1), 4);
  ASSERT_EQ(number::pow(number(), -0, 1), number(1));
  ASSERT_EQ(number::pow(number(1), -0, 1), number(1));
  ASSERT_EQ(number::pow(number(1), -2, 1), number(1));
  ASSERT_EQ(number::pow(number(2), -0, 1), number(1));
  ASSERT_EQ(number::pow(number(2), -1, 1), number(1, 2));
  ASSERT_EQ(number::pow(number(2), -2, 1), number(1, 4));
}

TEST(Number, Pow) {
  ASSERT_EQ(number::pow(number(), 1.1, 1), 0);
  ASSERT_EQ(number::pow(number(1), 1.5, 1), 1);
  ASSERT_EQ(number::pow(number(4), 2.5, 1), 32);
}
