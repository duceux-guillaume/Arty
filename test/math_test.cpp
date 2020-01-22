#include <gtest/gtest.h>

#include <arty/core/math.hpp>

using namespace arty;

TEST(Mat, Constructor) {
  ASSERT_EQ(Mat4x4f::identity(), Mat4x4f({
                                     1.f, 0.f, 0.f, 0.f,  //
                                     0.f, 1.f, 0.f, 0.f,  //
                                     0.f, 0.f, 1.f, 0.f,  //
                                     0.f, 0.f, 0.f, 1.f,  //
                                 }));
  ASSERT_EQ(Mat4x4f(), Mat4x4f({
                           0.f, 0.f, 0.f, 0.f,  //
                           0.f, 0.f, 0.f, 0.f,  //
                           0.f, 0.f, 0.f, 0.f,  //
                           0.f, 0.f, 0.f, 0.f,  //
                       }));
}

TEST(Mat, Getter) {
  Mat4x4f test{
      0, 4, 0, 0,  //
      2, 0, 0, 0,  //
      0, 0, 0, 0,  //
      0, 0, 0, 0,  //
  };
  ASSERT_EQ(test[1], 4.f);
  ASSERT_EQ(test(1, 0), 2.f);
}

TEST(Mat, Multiply) {
  ASSERT_EQ(Mat4x4f() * Mat4x4f(1.f), Mat4x4f());
  ASSERT_EQ(Mat4x4f(1.f) * Mat4x4f(1.f), Mat4x4f(1.f));
  ASSERT_EQ(Mat4x4f(2.f) * Mat4x4f(1.f), Mat4x4f(2.f));

  Mat4x4f line{
      1, 1, 1, 1,  //
      0, 0, 0, 0,  //
      1, 0, 0, 0,  //
      0, 0, 0, 0,  //
  };
  Mat4x4f col{
      1, 0, 1, 0,  //
      1, 0, 0, 0,  //
      1, 0, 0, 0,  //
      1, 0, 0, 0,  //
  };
  ASSERT_EQ(line * col, Mat4x4f({
                            4, 0, 1, 0,  //
                            0, 0, 0, 0,  //
                            1, 0, 1, 0,  //
                            0, 0, 0, 0,  //
                        }));
}

TEST(Vec, Eq) {
  ASSERT_EQ(Vec3f({1.f, 0.f, 0.f}), Vec3f({1.f, 0.f, 0.f}));
  ASSERT_NE(Vec3f({2.f, 0.f, 0.f}), Vec3f({1.f, 0.f, 0.f}));
  ASSERT_NE(Vec3f({-1.f, 0.f, 0.f}), Vec3f({1.f, 0.f, 0.f}));
}

TEST(Vec, Normalize) {
  Vec3f v1{1.f, 0.f, 0.f};
  ASSERT_EQ(v1.normalize(), v1);
  Vec3f v2{1.f, 0.f, 0.f};
  ASSERT_EQ(v2.normalize(), v1);
}

TEST(Vec, Cross) {
  ASSERT_EQ(cross(Vec3f{1.f, 0.f, 0.f}, Vec3f{0.f, 1.f, 0.f}),
            Vec3f({0.f, 0.f, 1.f}));
  ASSERT_EQ(cross(Vec3f{0.f, 1.f, 0.f}, Vec3f{0.f, 0.f, 1.f}),
            Vec3f({1.f, 0.f, 0.f}));
  ASSERT_EQ(cross(Vec3f{0.f, 0.f, 1.f}, Vec3f{0.f, 1.f, 0.f}),
            Vec3f({-1.f, 0.f, 0.f}));
  ASSERT_EQ(cross(Vec3f{0.f, 0.f, 1.f}, Vec3f{1.f, 0.f, 0.f}),
            Vec3f({0.f, 1.f, 0.f}));
}

TEST(Vec, Dot) {
  Vec3f test{1.f, 2.f, 3.f};
  ASSERT_EQ(test.dot(test), 14.f);
}

TEST(Mat, tr) {
  Mat4x4f i = Mat4x4f::identity();
  ASSERT_EQ(i.tr(), 4);
  Mat4x4f j;
  ASSERT_EQ(j.tr(), 0);
}

TEST(Mat, col) {
  Mat4x4f i(1.f);
  ASSERT_EQ(i.col(0), Vec4f(1.f, 0.f, 0.f, 0.f));
}

TEST(Mat4x4, inv) {
  Mat4x4f id = Mat4x4f::identity();
  ASSERT_EQ(id.inv(), id);
}

TEST(Mat, transpose) {
  Vec3f vec(1.f, 0.f, 0.f);
  auto tr = vec.transpose();
  ASSERT_EQ(tr[0], 1.f);
  ASSERT_EQ(tr[1], 0.f);
  ASSERT_EQ(tr[2], 0.f);
}

TEST(Mat, Multiply2) {
  Vec2f vec(1.f, 2.f);
  Vec2f::transpose_type vecT = vec.transpose();
  ASSERT_EQ(vec * vecT, Mat2x2f(1, 2, 2, 4));
  ASSERT_EQ(vecT * vec, Mat1f(5));
}

TEST(Mat, Apply) {
  Vec2f vec(-1.f, -2.f);
  Vec2f r1 = vec.apply([](float f) { return std::abs(f); });
  ASSERT_EQ(r1, Vec2f(1.f, 2.f));
}

TEST(Mat, ApplyWith) {
  Vec2f v1(1.f, 2.f);
  Vec2f v2(2.f, 1.f);
  Vec2f r1 = v1.apply(v2, [](float f1, float f2) { return std::min(f1, f2); });
  ASSERT_EQ(r1, Vec2f(1.f, 1.f));
}

TEST(Mat, SelfMinus) {
  Vec2f vec(1.f, 2.f);
  ASSERT_EQ(-vec, Vec2f(-1.f, -2.f));
  ASSERT_EQ(-(-vec), vec);
}

TEST(Mat, verify) {
  Vec2f vec(1.f, 2.f);
  ASSERT_TRUE(vec.verify([](float f) { return f > 0.f; }));
  ASSERT_FALSE(vec.verify([](float f) { return f <= 0.f; }));
}
