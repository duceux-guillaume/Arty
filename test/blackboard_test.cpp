#include <arty/core/blackboard.h>
#include <gtest/gtest.h>

using namespace arty;

TEST(Blackboard, createEntity) {
  Blackboard board;
  auto e1 = board.createEntity("player");
  auto e2 = board.createEntity("player");
  ASSERT_EQ(e1, Entity("player_1", 1));
  ASSERT_NE(e1, e2);
}

TEST(Blackboad, SetGet) {
  Blackboard board;
  Entity world = board.createEntity("world");
  board.set(world, "position", Vec3f());
  Entity player = board.createEntity("player");
  board.set(player, "position", Vec3f({1, 2, 3}));
  auto ptr = board.getProperties<Vec3f>("position");
  ASSERT_TRUE(ptr);
  ASSERT_EQ(ptr->size(), 2);
  auto ptr2 = board.getEntityProperty<Vec3f>(player, "position");
  ASSERT_TRUE(ptr2);
  ASSERT_EQ(*ptr2, Vec3f({1, 2, 3}));
}

TEST(Blackboad, SetGet2) {
  Blackboard board;
  Entity world = board.createEntity("world");
  board.set(world, "position", Vec3f());
  auto ptr = board.getEntityProperty<Vec3f>(world, "position");
  ASSERT_TRUE(ptr);
  board.set(world, "position", Vec3f(1.f));
}

TEST(Blackboad, ClearProperties) {
  Blackboard board;
  Entity player = board.createEntity("player");
  board.set(player, "position", Vec3f());
  auto prop = board.getProperties<Vec3f>("position");
  ASSERT_EQ(prop->size(), 1);
  ASSERT_EQ(prop->at(0).entity, player);
  board.clearProperties("position");
  ASSERT_EQ(prop->size(), 0);
  ASSERT_EQ(prop, board.getProperties<Vec3f>("position"));
}

TEST(Blackboard, Iterator) {
  Blackboard board;
  board.set(Entity("toto", 1), "position", Vec3f());
  board.set(Entity("toto", 10), "position", Vec3f());
  auto prop = board.getProperties<Vec3f>("position");
  std::size_t count = 0;
  for (auto p : *prop) {
    ASSERT_TRUE(p.entity.isValid());
    count++;
  }
  ASSERT_EQ(count, 2);
}
