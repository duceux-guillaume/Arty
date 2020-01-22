#include <gtest/gtest.h>

#include <arty/impl/collision.hpp>
#include <arty/impl/mesh_loader_system.hpp>

using namespace arty;

TEST(CollisionDetection, detect) {
  Vec3f center = Vec3f();
  ASSERT_EQ(center, Vec3f());
}
