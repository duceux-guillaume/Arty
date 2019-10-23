#include <arty/core/ecs.h>
#include <arty/impl/engine.h>
#include <gtest/gtest.h>

using namespace arty;

TEST(Entity, Create) {
  Entity e1 = Entity("toto");
  Entity e2 = Entity("tata");
  EXPECT_NE(e1, e2);
  EXPECT_EQ(e1, e1);
}
