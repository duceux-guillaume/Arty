#include <arty/core/number.h>

#include <gtest/gtest.h>

using arty::Number;

TEST(Number, Greater) {
  ASSERT_TRUE(Number(1) > Number(0));
  ASSERT_TRUE(Number(10) > Number(1));
  ASSERT_FALSE(Number(1) > Number(10));
  ASSERT_FALSE(Number(0) > Number(1));
  ASSERT_FALSE(Number(1) > Number(1));
  ASSERT_FALSE(Number(11) > Number(12));
  ASSERT_FALSE(Number(123456) > Number(123456));
  ASSERT_TRUE(Number(12) > Number(11));
}

TEST(Number, Different) {
  ASSERT_TRUE(Number(1) != Number(0));
  ASSERT_TRUE(Number(10) != Number(1));
  ASSERT_FALSE(Number(123456) != Number(123456));
  ASSERT_TRUE(Number(1) != Number(10));
}

TEST(Number, Less) {
  ASSERT_FALSE(Number(1) < Number(0));
  ASSERT_FALSE(Number(10) < Number(1));
  ASSERT_TRUE(Number(1) < Number(10));
  ASSERT_TRUE(Number(0) < Number(1));
  ASSERT_FALSE(Number(1) < Number(1));
  ASSERT_TRUE(Number(11) < Number(12));
  ASSERT_FALSE(Number(123456) < Number(123456));
  ASSERT_FALSE(Number(12) < Number(11));
}

TEST(Number, Equal) {
  ASSERT_FALSE(Number(1) == Number(0));
  ASSERT_FALSE(Number(10) == Number(1));
  ASSERT_TRUE(Number(123456) == Number(123456));
  ASSERT_FALSE(Number(1) == Number(10));
}

TEST(Number, GreaterOrEqual) {
  ASSERT_TRUE(Number(1) >= Number(0));
  ASSERT_TRUE(Number(10) >= Number(1));
  ASSERT_FALSE(Number(1) >= Number(10));
  ASSERT_FALSE(Number(0) >= Number(1));
  ASSERT_TRUE(Number(1) >= Number(1));
  ASSERT_FALSE(Number(11) >= Number(12));
  ASSERT_TRUE(Number(123456) >= Number(123456));
  ASSERT_TRUE(Number(12) >= Number(11));
}

TEST(Number, LessOrEqual) {
  ASSERT_FALSE(Number(1) <= Number(0));
  ASSERT_FALSE(Number(10) <= Number(1));
  ASSERT_TRUE(Number(1) <= Number(10));
  ASSERT_TRUE(Number(0) <= Number(1));
  ASSERT_TRUE(Number(1) <= Number(1));
  ASSERT_TRUE(Number(11) <= Number(12));
  ASSERT_TRUE(Number(123456) <= Number(123456));
  ASSERT_FALSE(Number(12) <= Number(11));
}

TEST(Number, Add) {
  ASSERT_EQ(Number(0) + Number(0), Number(0));
  ASSERT_EQ(Number(0) + Number(), Number(0));
  ASSERT_EQ(Number(10) + Number(), Number(10));
  ASSERT_EQ(Number(19) + Number(1), Number(20));
  ASSERT_EQ(Number(15) + Number(4), Number(4) + Number(15));
  ASSERT_EQ(Number(20, 3) + Number(10, 3), Number(10, 1));
}

TEST(Number, Mult) {
  ASSERT_EQ(Number(0) * Number(0), Number(0));
  ASSERT_EQ(Number(0) * Number(), Number(0));
  ASSERT_EQ(Number(10) * Number(0), Number());
  ASSERT_EQ(Number(10) * Number(), Number());
  ASSERT_EQ(Number(1) * Number(11), Number(11));
  ASSERT_EQ(Number(2) * Number(19), Number(38));
  ASSERT_EQ(Number(1) * Number(0), Number(0));
  ASSERT_EQ(Number(2) * Number(2), Number(4));
  ASSERT_EQ(Number(15) * Number(4), Number(4) * Number(15));
  ASSERT_EQ(Number(3, 10) * Number(10000), Number(3000, 1));
}

TEST(Number, Neg) {
  ASSERT_TRUE(Number(-1) < 0);
  ASSERT_TRUE(-Number(1) < 0);
  ASSERT_FALSE(-Number(-1) < 0);
}

TEST(Number, Equality) {
  ASSERT_EQ(Number(1), Number(1));
  ASSERT_EQ(Number(0), Number());
  ASSERT_EQ(Number(10), Number(10));
  ASSERT_NE(Number(10), Number(0));
}

TEST(Number, Minus) {
  ASSERT_EQ(Number(1), -(-Number(1)));
  ASSERT_EQ(-Number(10), Number(-10));
  ASSERT_NE(Number(1), -Number(1));
  ASSERT_EQ(Number(1) + (-Number(1)), Number());
  ASSERT_EQ(-Number(1) + Number(1), 0);
}

TEST(Number, Sub) {
  ASSERT_EQ(Number(1) - Number(1), 0);
  ASSERT_EQ(-Number(1) - Number(1), -2);
  ASSERT_EQ(Number(10) - Number(1), 9);
  ASSERT_EQ(Number(1) - Number(10), -9);
  ASSERT_EQ(Number(3000) - Number(300), 2700);
}

TEST(Number, Constructor) {
  ASSERT_EQ(Number(50, 40), Number(5, 4));
  ASSERT_EQ(Number(-10, 5), Number(-2, 1));
  ASSERT_EQ(Number(3000, 100), Number(30, 1));
}

TEST(Number, Div) {
  ASSERT_EQ(Number(1, 1) / Number(1, 1), Number(1, 1));
  ASSERT_EQ(Number(1, 2) / Number(1, 1), Number(1, 2));
  ASSERT_EQ(Number(-1, 2) / Number(1, 1), Number(-1, 2));
  ASSERT_EQ(Number(-1, 2) / Number(1, 2), Number(-1, 1));
}

TEST(Number, BenchAddition) {
  Number toto(1, 10);
  for (int i = 0; i <= 1000; ++i) {
    toto += Number(i);
  }
  std::cout << toto << std::endl;
}

TEST(GroundTruth, BenchAddition) {
  double toto = 0.1;
  for (int i = 0; i <= 1000; ++i) {
    toto += i;
  }
  std::cout << toto << std::endl;
}

TEST(Number, BenchMult) {
  Number toto(1, 10);
  for (int i = 1; i <= 10000; ++i) {
    toto = Number(3, 10) * Number(i);
  }
  std::cout << toto << std::endl;
  ASSERT_EQ(toto, Number(3000, 1));
}

TEST(GroundTruth, BenchMult) {
  double toto = 0.1;
  for (int i = 1; i <= 10000; ++i) {
    toto = 0.3 * i;
  }
  std::cout << toto << std::endl;
}
