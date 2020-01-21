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
  AABox2f unit(Vec2f(), Vec2f(1.f, 1.f));
  AABox2f random(Vec2f(1.5f, 0.f), Vec2f(1.f, 1.f));
  ASSERT_TRUE(unit.intersect(random));
  ASSERT_TRUE(random.intersect(unit));
}
