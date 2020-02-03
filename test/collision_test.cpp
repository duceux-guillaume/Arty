#include <gtest/gtest.h>

#include <arty/impl/physics.hpp>

using namespace arty;

TEST(CollisionDetection, detect) {
  Tf3f tf1;
  Tf3f tf2(Vec3f{1.5f, 0.f, 0.f});
  AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
  CollisionDetection detector;
  auto collision = detector.detect(tf1, box, tf2, box);
  ASSERT_TRUE(collision.exist());
}
