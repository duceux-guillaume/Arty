#include <arty/impl/matrix.h>

#include <gtest/gtest.h>

using arty::Matrix;
using arty::Number;

TEST(Matrix, Constructor) {
  Matrix zero_scalar(1);
  ASSERT_EQ(zero_scalar(0, 0), Number(0));
}

TEST(Matrix, ChangeVal) {
  Matrix zero_scalar(1);
  zero_scalar(0, 0) = 1;
  ASSERT_EQ(zero_scalar(0, 0), 1);
}

TEST(Matrix, Addition) {
  Matrix zero(1);
  Matrix one(1);
  one(0, 0) = 1;
  Matrix res = zero + one;
  ASSERT_EQ(res(0, 0), 1);
  std::cout << res << std::endl;

  Matrix square(2, 2);
  square(0, 0) = 3;
  square(0, 1) = 3;
  square(1, 0) = 3;
  square(1, 1) = 3;

  Matrix identity(2, 2);
  identity(0, 0) = 1;
  identity(0, 1) = 0;
  identity(1, 0) = 0;
  identity(1, 1) = 1;

  Matrix res2 = square + identity;
  ASSERT_EQ(res2(0, 0), 4);
  ASSERT_EQ(res2(0, 1), 3);
  ASSERT_EQ(res2(1, 0), 3);
  ASSERT_EQ(res2(1, 1), 4);
  std::cout << res2 << std::endl;
}

TEST(Matrix, Multiplication) {
  Matrix zero(1);
  Matrix one(1);
  one(0, 0) = 1;
  Matrix res = zero * one;
  ASSERT_EQ(res(0, 0), 0);
  std::cout << res << std::endl;

  Matrix square(2, 2);
  square(0, 0) = 3;
  square(0, 1) = 3;
  square(1, 0) = 3;
  square(1, 1) = 3;

  Matrix identity(2, 2);
  identity(0, 0) = 1;
  identity(0, 1) = 0;
  identity(1, 0) = 0;
  identity(1, 1) = 1;

  Matrix res2 = square * identity;
  ASSERT_EQ(res2(0, 0), 3);
  ASSERT_EQ(res2(0, 1), 3);
  ASSERT_EQ(res2(1, 0), 3);
  ASSERT_EQ(res2(1, 1), 3);
  std::cout << res2 << std::endl;
}

TEST(Matrix, ScalarProd) {
  Matrix one(1, 10);
  Matrix two(10, 1);
  one += 1;
  two += 2;
  std::cout << one << std::endl;
  std::cout << two << std::endl;
  Matrix res = one * two;
  ASSERT_EQ(res.dim().rows(), 1);
  ASSERT_EQ(res.dim().cols(), 1);
  ASSERT_EQ(res(0, 0), 20);
  std::cout << res << std::endl;
}

TEST(Matrix, VectorVector) {
  Matrix one(1, 10);
  Matrix two(10, 1);
  one += 1;
  two += 2;
  std::cout << one << std::endl;
  std::cout << two << std::endl;
  Matrix res = two * one;
  ASSERT_EQ(res.dim().rows(), 10);
  ASSERT_EQ(res.dim().cols(), 10);
  ASSERT_EQ(res(0, 0), 2);
  std::cout << res << std::endl;
}

TEST(Matrix, Bench) {
  Matrix zero(1000, 10000);
  Matrix one(10000, 1);
  one += 1;
  Matrix res = zero * one;
}
