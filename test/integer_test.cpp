#include <arty/math/core/number.h>

#include <gtest/gtest.h>

using arty::math::Whole;
using arty::math::Integer;

TEST(Integer, Neg) {
    ASSERT_TRUE(Integer(-1) < 0);
    ASSERT_TRUE(-Integer(1) < 0);
    ASSERT_FALSE(-Integer(-1) < 0);
    ASSERT_TRUE((-Integer(1)).neg());
    ASSERT_FALSE(Integer(0).neg());
    ASSERT_FALSE(-Integer(0).neg());
}

TEST(IntegerComp, Equality) {
    ASSERT_EQ(Integer(1), Integer(1));
    ASSERT_EQ(Integer(0), Integer());
    ASSERT_EQ(Integer(10), Integer(10));
    ASSERT_NE(Integer(10), Integer(0));
}

TEST(IntegerOp, Minus) {
    ASSERT_EQ(Integer(1), -(-Integer(1)));
    ASSERT_EQ(-Integer(10), Integer(-10));
    ASSERT_NE(Integer(1), -Integer(1));
    ASSERT_EQ(Integer(1)+(-Integer(1)), Integer());
    ASSERT_EQ(-Integer(1)+Integer(1), 0);
}

TEST(IntegerOp, Sub) {
    ASSERT_EQ(Integer(1) - Integer(1), 0);
    ASSERT_EQ(-Integer(1) - Integer(1), -2);
    ASSERT_EQ(Integer(10) - Integer(1), 9);
    ASSERT_EQ(Integer(1) - Integer(10), -9);
}

TEST(IntegerOp, Add) {
    ASSERT_EQ(Integer(1) + Integer(1), 2);
    ASSERT_EQ(-Integer(1) + Integer(1), 0);
    ASSERT_EQ(Integer(10) + Integer(1), Integer(11));
    ASSERT_EQ(Integer(1) + (-Integer(10)), -9);
}

TEST(IntegerOp, Mult) {
    ASSERT_EQ(Integer(1) * Integer(1), 1);
    ASSERT_EQ(-Integer(1) * Integer(1), -1);
    ASSERT_EQ(Integer(10) * Integer(2), Integer(20));
    ASSERT_EQ(-Integer(1) * Integer(-1), 1);
}
