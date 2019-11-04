#include <gtest/gtest.h>

#include <arty/impl/collision.hpp>
#include <arty/impl/mesh_loader_system.hpp>

using namespace arty;

static Mesh makeCube() {
  Mesh m;
  Loader l;
  EXPECT_TRUE(l.loadObj("test_cube.obj", &m));
  return m;
}

TEST(CollisionDetection, computeCenter) {
  CollisionDetection detector;
  Vec3f center = detector.computeCenter(makeCube());
  ASSERT_EQ(center, Vec3f());
}

TEST(CollisionDetection, computeOuterCircle) {
  CollisionDetection detector;
  Sphere circle = detector.computeOuterCircle(makeCube());
  ASSERT_EQ(circle.center, Vec3f());
  ASSERT_EQ(circle.squaredRadius, 3.f);
}

TEST(CollisionDetection, computeInnerCircle) {
  CollisionDetection detector;
  Sphere circle = detector.computeInnerCircle(makeCube());
  ASSERT_EQ(circle.center, Vec3f());
  ASSERT_EQ(circle.squaredRadius, 1.f);
}
