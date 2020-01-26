#include <gtest/gtest.h>

#include <arty/core/geometry.hpp>

using namespace arty;

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
  Trianglef triangle(Vec3f(0.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f),
                     Vec3f(0.f, 1.f, 0.f));
  Vec3f p = Vec3f(0.f, 0.f, 1.f);
  Vec3f res = triangle.project(p);
  Vec3f exp(0.f, 0.f, 0.f);
  ASSERT_EQ(res, exp);
  ASSERT_EQ(triangle.distanceSquaredTo(p), 1.f);
}

// TEST(Triangle, intersect) {
//  Trianglef triangle(Vec3f(0.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f),
//                     Vec3f(0.f, 1.f, 0.f));
//  Edge3f edgez(Vec3f(2.f, 0.f, -1.f), Vec3f(2.f, 0.f, 1.f));
//  Intersection<Vec3f> res = triangle.intersect(edgez);
//  ASSERT_FALSE(res.exist);
//  Edge3f ori(Vec3f(0.f, 0.f, -1.f), Vec3f(0.f, 0.f, 1.f));
//  res = triangle.intersect(ori);
//  ASSERT_TRUE(res.exist);
//  ASSERT_EQ(res.value, Vec3f());
//}

TEST(AABox2f, intersect) {
  {
    AABox2f unit(Vec2f(), Vec2f(1.f, 1.f));
    AABox2f random(Vec2f(1.5f, 0.f), Vec2f(1.f, 1.f));
    ASSERT_TRUE(unit.intersect(random));
    ASSERT_TRUE(random.intersect(unit));
  }
  {
    AABox2f unit(Vec2f(), Vec2f(1.f, 1.f));
    AABox2f random(Vec2f(2.5f, 0.f), Vec2f(1.f, 1.f));
    ASSERT_FALSE(unit.intersect(random));
    ASSERT_FALSE(random.intersect(unit));
  }
}

TEST(AABox2f, intersection) {
  {
    AABox2f unit(Vec2f(), Vec2f(1.f, 1.f));
    AABox2f random(Vec2f(1.5f, 0.f), Vec2f(1.f, 1.f));
    auto intersection = unit.intersection(random);
    ASSERT_TRUE(intersection.exist());
    ASSERT_EQ(intersection.value().halfLength(), Vec2f(0.25f, 1.f));
    ASSERT_EQ(intersection.value().center(), Vec2f(0.75f, 0.f));
  }
  {
    AABox2f unit(Vec2f(), Vec2f(1.f, 1.f));
    AABox2f random(Vec2f(-1.5f, 1.f), Vec2f(1.f, 0.5f));
    auto intersection = unit.intersection(random);
    ASSERT_TRUE(intersection.exist());
    ASSERT_EQ(intersection.value().halfLength(), Vec2f(0.25f, 0.25f));
    ASSERT_EQ(intersection.value().center(), Vec2f(-0.75f, 0.75f));
  }
  {
    AABox3f unit(Vec3f::zero(), Vec3f::all(1.f));
    AABox3f random(Vec3f(0.f, -1.5f, 0.f), Vec3f(2.f, 1.f, 2.f));
    auto intersection = unit.intersection(random);
    ASSERT_TRUE(intersection.exist());
    ASSERT_EQ(intersection.value().halfLength(), Vec3f(1.f, 0.25f, 1.f));
    ASSERT_EQ(intersection.value().center(), Vec3f(0.f, -0.75f, 0.f));
  }
}

TEST(Tf3f, toMat) {
  {
    Tf3f tf;
    Mat4x4f identity{
        1.f, 0.f, 0.f, 0.f,  //
        0.f, 1.f, 0.f, 0.f,  //
        0.f, 0.f, 1.f, 0.f,  //
        0.f, 0.f, 0.f, 1.f   //
    };
    ASSERT_EQ(tf.toMat(), identity);
  }
  {
    Tf3f tf(Vec3f{1.f, 2.f, 3.f});
    Mat4x4f translation{
        1.f, 0.f, 0.f, 1.f,  //
        0.f, 1.f, 0.f, 2.f,  //
        0.f, 0.f, 1.f, 3.f,  //
        0.f, 0.f, 0.f, 1.f   //
    };
    ASSERT_EQ(tf.toMat(), translation);
  }
  {
    Mat3x3f rotation{
        0.f, -1.f, 0.f,  //
        1.f, 0.f,  0.f,  //
        0.f, 0.f,  1.f   //
    };
    Tf3f tf(Vec3f{1.f, 2.f, 3.f}, rotation);
    Mat4x4f transform{
        0.f, -1.f, 0.f, 1.f,  //
        1.f, 0.f,  0.f, 2.f,  //
        0.f, 0.f,  1.f, 3.f,  //
        0.f, 0.f,  0.f, 1.f   //
    };
    ASSERT_EQ(tf.toMat(), transform);
  }
}

TEST(Geo, intersectLinePlane) {
  Line3f xaxis(Vec3f(), Vec3f(1.f, 0.f, 0.f));
  Plane3f xyplane(Vec3f(), Vec3f(1.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f));
  ASSERT_FALSE(Geo::intersect(xaxis, xyplane).exist());
  Plane3f zyplane(Vec3f(), Vec3f(0.f, 0.f, 1.f), Vec3f(0.f, 1.f, 0.f));
  auto zero = Geo::intersect(xaxis, zyplane);
  ASSERT_TRUE(zero.exist());
  ASSERT_EQ(zero.value(), Vec3f());
}

TEST(Geo, AABoxContainsPoint) {
  auto unit = AABox2f::unit();
  ASSERT_TRUE(Geo::contains(unit, unit.center()));
  ASSERT_TRUE(Geo::contains(unit, unit.center() + unit.halfLength()));
  ASSERT_FALSE(Geo::contains(unit, unit.center() + unit.halfLength() * 2.f));
}
