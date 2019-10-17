#include <arty/core/ecs.h>

#include <gtest/gtest.h>

using namespace arty;

TEST(Entity, Create) {
  Entity e1 = Entity::create();
  Entity e2 = Entity::create();
  EXPECT_NE(e1, e2);
  EXPECT_EQ(e1, e1);
}

TEST(Engine, SpawnEntity) {
  Engine engine;
  ASSERT_TRUE(engine.step());
}
