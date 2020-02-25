#include <gtest/gtest.h>

#include <arty/core/dynamic_matrix.hpp>

using namespace arty;

TEST(Matrix, Constructor) {
  ASSERT_EQ(Matrix::identity(4), Matrix(4, 4,
                                        {
                                            1.f, 0.f, 0.f, 0.f,  //
                                            0.f, 1.f, 0.f, 0.f,  //
                                            0.f, 0.f, 1.f, 0.f,  //
                                            0.f, 0.f, 0.f, 1.f,  //
                                        }));
  ASSERT_EQ(Matrix(4, 4), Matrix(4, 4,
                                 {
                                     0.f, 0.f, 0.f, 0.f,  //
                                     0.f, 0.f, 0.f, 0.f,  //
                                     0.f, 0.f, 0.f, 0.f,  //
                                     0.f, 0.f, 0.f, 0.f,  //
                                 }));
}

TEST(Mat, Getter) {
  Matrix test(4, 4,
              {
                  0, 4, 0, 0,  //
                  2, 0, 0, 0,  //
                  0, 0, 0, 0,  //
                  0, 0, 0, 0,  //
              });
  ASSERT_EQ(test[1], 4.f);
  ASSERT_EQ(test(1, 0), 2.f);
}

TEST(Mat, Multiply) {
  ASSERT_EQ(Matrix(4, 4) * Matrix(4, 4, 1.f), Matrix(4, 4));
  ASSERT_EQ(Matrix::identity(4) * Matrix::identity(4), Matrix::identity(4));
  ASSERT_EQ(Matrix::diagonal(4, 2) * Matrix::identity(4),
            Matrix::diagonal(4, 2));

  Matrix line(4, 4,
              {
                  1, 1, 1, 1,  //
                  0, 0, 0, 0,  //
                  1, 0, 0, 0,  //
                  0, 0, 0, 0,  //
              });
  Matrix col(4, 4,
             {
                 1, 0, 1, 0,  //
                 1, 0, 0, 0,  //
                 1, 0, 0, 0,  //
                 1, 0, 0, 0,  //
             });
  ASSERT_EQ(line * col, Matrix(4, 4,
                               {
                                   4, 0, 1, 0,  //
                                   0, 0, 0, 0,  //
                                   1, 0, 1, 0,  //
                                   0, 0, 0, 0,  //
                               }));
}

TEST(Mat, Addition) {
  ASSERT_EQ(Matrix(4, 4, 1) + Matrix(4, 4, 1), Matrix(4, 4, 2));
}

TEST(Mat, Substraction) {
  ASSERT_EQ(Matrix(4, 4) - Matrix(4, 4, 1), Matrix(4, 4, -1));
}

TEST(Mat, MultiplyScaler) { ASSERT_EQ(Matrix(4, 4, 1) * 2, Matrix(4, 4, 2)); }

TEST(Mat, Norm) {
  auto I = Matrix(2, 1, {1, 0});
  ASSERT_EQ(I.norm(), 1);
}

TEST(Mat, Transpose) {
  ASSERT_EQ(Matrix(2, 1, {1, 0}).transpose(), Matrix(1, 2, {1, 0}));
  ASSERT_EQ(Matrix(2, 2, {0, 0, 1, 0}).transpose(), Matrix(2, 2, {0, 1, 0, 0}));
}

TEST(Mat, UniryMinus) { ASSERT_EQ(-Matrix(2, 1, 1), Matrix(1, 2, -1)); }

TEST(Mat, Ostream) {
  ASSERT_NO_THROW(std::cout << Matrix::identity(4) << std::endl);
}
/*
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

TEST(Mat, block) {
  auto id4 = Mat4x4f::identity();
  auto block = id4.block<float, 2, 2>(0, 0);
  ASSERT_EQ(block, Mat2x2f::identity());
}

TEST(Mat, setBlock) {
  Mat4x4f block;
  block.setBlock(0, 0, Mat2x2f::identity());
  block.setBlock(2, 2, Mat2x2f::identity());
  ASSERT_EQ(block, Mat4x4f::identity());
}

TEST(Vec, grow) {
  Vec3f pt(0.f, 1.f, 2.f);
  ASSERT_EQ(Vec4f(pt, 3.f), Vec4f(0.f, 1.f, 2.f, 3.f));
}

TEST(Vec, cast) {
  Vec3f pt(0.f, 1.f, 2.f);
  ASSERT_EQ(Vec3d(pt), Vec3d(0., 1., 2.));
}
*/
