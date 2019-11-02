#include <arty/core/math.h>
#include <gtest/gtest.h>

using namespace arty;

TEST(Mat, Constructor) {
  ASSERT_EQ(Mat4x4f(1.f), Mat4x4f({
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
  ASSERT_EQ(normalize(Vec3f{1.f, 0.f, 0.f}), Vec3f({1.f, 0.f, 0.f}));
  ASSERT_EQ(normalize(Vec3f{2.f, 0.f, 0.f}), Vec3f({1.f, 0.f, 0.f}));
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
  ASSERT_EQ(dot(test, test), 14.f);
}

TEST(Line, project) {
  Line3f line(Vec3f(1.f, 1.f, 0.f), Vec3f(-1.f, 1.f, 0.f));
  Vec3f res = line.project(Vec3f());
  ASSERT_EQ(res, Vec3f(0.f, 1.f, 0.f));
  ASSERT_EQ(line.distanceSquaredTo(Vec3f()), 1.f);
}

TEST(Edge, projectPointInTheMidle) {
  Edge3f edge(Vec3f(1.f, 1.f, 0.f), Vec3f(-1.f, 1.f, 0.f));
  Vec3f res = edge.project(Vec3f());
  ASSERT_EQ(res, Vec3f(0.f, 1.f, 0.f));
  ASSERT_EQ(edge.distanceSquaredTo(Vec3f()), 1.f);
}

TEST(Edge, projectPointLeft) {
  Edge3f edge(Vec3f(1.f, 0.f, 0.f), Vec3f(2.f, 0.f, 0.f));
  Vec3f res = edge.project(Vec3f());
  ASSERT_EQ(res, Vec3f(1.f, 0.f, 0.f));
  ASSERT_EQ(edge.distanceSquaredTo(Vec3f()), 1.f);
}

TEST(Edge, projectPointRight) {
  Edge3f edge(Vec3f(-2.f, 0.f, 0.f), Vec3f(-1.f, 0.f, 0.f));
  Vec3f res = edge.project(Vec3f());
  ASSERT_EQ(res, Vec3f(-1.f, 0.f, 0.f));
  ASSERT_EQ(edge.distanceSquaredTo(Vec3f()), 1.f);
}

TEST(Plane, project) {
  Plane3f plane(Vec3f(0.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f),
                Vec3f(0.f, 1.f, 0.f));
  Vec3f p = Vec3f(0.f, 0.f, 1.f);
  Vec3f res = plane.project(p);
  Vec3f exp(0.f, 0.f, 0.f);
  ASSERT_EQ(res, exp);
  ASSERT_EQ(plane.distanceSquaredTo(p), 1.f);
}

TEST(Triangle, project) {
  Trianglef plane(Vec3f(0.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f),
                  Vec3f(0.f, 1.f, 0.f));
  Vec3f p = Vec3f(0.f, 0.f, 1.f);
  Vec3f res = plane.project(p);
  Vec3f exp(0.f, 0.f, 0.f);
  ASSERT_EQ(res, exp);
  ASSERT_EQ(plane.distanceSquaredTo(p), 1.f);
}
