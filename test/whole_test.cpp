#include <arty/math/core/number.h>

#include <gtest/gtest.h>

using arty::math::Whole;

TEST(WholeComp, Greater) {
    ASSERT_TRUE(Whole(1) > Whole(0));
    ASSERT_TRUE(Whole(10) > Whole(1));
    ASSERT_FALSE(Whole(1) > Whole(10));
    ASSERT_FALSE(Whole(0) > Whole(1));
    ASSERT_FALSE(Whole(1) > Whole(1));
    ASSERT_FALSE(Whole(11) > Whole(12));
    ASSERT_FALSE(Whole(123456) > Whole(123456));
    ASSERT_TRUE(Whole(12) > Whole(11));
}

TEST(WholeComp, Different) {
    ASSERT_TRUE(Whole(1) != Whole(0));
    ASSERT_TRUE(Whole(10) != Whole(1));
    ASSERT_FALSE(Whole(123456) != Whole(123456));
    ASSERT_TRUE(Whole(1) != Whole(10));
}

TEST(WholeComp, Less) {
    ASSERT_FALSE(Whole(1) < Whole(0));
    ASSERT_FALSE(Whole(10) < Whole(1));
    ASSERT_TRUE(Whole(1) < Whole(10));
    ASSERT_TRUE(Whole(0) < Whole(1));
    ASSERT_FALSE(Whole(1) < Whole(1));
    ASSERT_TRUE(Whole(11) < Whole(12));
    ASSERT_FALSE(Whole(123456) < Whole(123456));
    ASSERT_FALSE(Whole(12) < Whole(11));
}

TEST(WholeComp, Equal) {
    ASSERT_FALSE(Whole(1) == Whole(0));
    ASSERT_FALSE(Whole(10) == Whole(1));
    ASSERT_TRUE(Whole(123456) == Whole(123456));
    ASSERT_FALSE(Whole(1) == Whole(10));
}

TEST(WholeComp, GreaterOrEqual) {
    ASSERT_TRUE(Whole(1) >= Whole(0));
    ASSERT_TRUE(Whole(10) >= Whole(1));
    ASSERT_FALSE(Whole(1) >= Whole(10));
    ASSERT_FALSE(Whole(0) >= Whole(1));
    ASSERT_TRUE(Whole(1) >= Whole(1));
    ASSERT_FALSE(Whole(11) >= Whole(12));
    ASSERT_TRUE(Whole(123456) >= Whole(123456));
    ASSERT_TRUE(Whole(12) >= Whole(11));
}

TEST(WholeComp, LessOrEqual) {
    ASSERT_FALSE(Whole(1) <= Whole(0));
    ASSERT_FALSE(Whole(10) <= Whole(1));
    ASSERT_TRUE(Whole(1) <= Whole(10));
    ASSERT_TRUE(Whole(0) <= Whole(1));
    ASSERT_TRUE(Whole(1) <= Whole(1));
    ASSERT_TRUE(Whole(11) <= Whole(12));
    ASSERT_TRUE(Whole(123456) <= Whole(123456));
    ASSERT_FALSE(Whole(12) <= Whole(11));
}

TEST(WholeOp, Add) {
    ASSERT_EQ(Whole(0) + Whole(0), Whole(0));
    ASSERT_EQ(Whole(0) + Whole(), Whole(0));
    ASSERT_EQ(Whole(10) + Whole(), Whole(10));
    ASSERT_EQ(Whole(19) + Whole(1), Whole(20));
    ASSERT_EQ(Whole(15) + Whole(4), Whole(4) + Whole(15));
}

TEST(WholeOp, Mult) {
    ASSERT_EQ(Whole(0) * Whole(0), Whole(0));
    ASSERT_EQ(Whole(0) * Whole(), Whole(0));
    ASSERT_EQ(Whole(10) * Whole(0), Whole());
    ASSERT_EQ(Whole(10) * Whole(), Whole());
    ASSERT_EQ(Whole(1) * Whole(11), Whole(11));
    ASSERT_EQ(Whole(2) * Whole(19), Whole(38));
    ASSERT_EQ(Whole(1) * Whole(0), Whole(0));
    ASSERT_EQ(Whole(2) * Whole(2), Whole(4));
    ASSERT_EQ(Whole(15) * Whole(4), Whole(4) * Whole(15));
}
