#include <arty/core/blackboard.h>
#include <gtest/gtest.h>

using namespace arty;

TEST(Blackboard, createEntity) {
  Blackboard board;
  ASSERT_EQ(board.createEntity("player"), "player");
  ASSERT_NE(board.createEntity("player"), "player");
}

TEST(Blackboad, SetGet) {
  Blackboard board;
  std::string world = board.createEntity("world");
  board.set(world, "position", Vec3f());
  std::string player = board.createEntity("player");
  board.set(player, "position", Vec3f({1, 2, 3}));
  auto ptr = board.getProperties<Vec3f>("position");
  ASSERT_TRUE(ptr);
  ASSERT_TRUE(ptr->buffer().size() == 2);
  auto ptr2 = board.getEntityProperty<Vec3f>(player, "position");
  ASSERT_TRUE(ptr2);
  ASSERT_EQ(*ptr2, Vec3f({1, 2, 3}));
}
