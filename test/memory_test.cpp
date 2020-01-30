#include <gtest/gtest.h>

#include <arty/core/math.hpp>
#include <arty/core/memory.hpp>

using namespace arty;

TEST(Memory, createEntity) {
  Memory board;
  auto e1 = board.createEntity("player");
  auto e2 = board.createEntity("player");
  ASSERT_EQ(e1, Entity("player_1", 1));
  ASSERT_NE(e1, e2);
}

TEST(Memory, write) {
  Memory mem;
  ASSERT_TRUE(mem.write(mem.createEntity("toto"), 10));
}

TEST(Memory, SetGet) {
  Memory mem;
  Entity world = mem.createEntity("world");
  mem.write(world, Vec3f());
  Entity player = mem.createEntity("player");
  mem.write(player, Vec3f({1, 2, 3}));
  ASSERT_EQ(mem.count<Vec3f>(), 2);
  Vec3f val;
  ASSERT_TRUE(mem.read<Vec3f>(player, val));
  ASSERT_EQ(val, Vec3f({1, 2, 3}));
}

TEST(Memory, SetGet2) {
  Memory board;
  Entity world = board.createEntity("world");
  board.write(world, Vec3f());
  Vec3f val;
  ASSERT_TRUE(board.read<Vec3f>(world, val));
  ASSERT_EQ(val, Vec3f());
  ASSERT_TRUE(board.write(world, Vec3f(1.f)));
  ASSERT_TRUE(board.read<Vec3f>(world, val));
  ASSERT_EQ(val, Vec3f(1.f));
}

TEST(Memory, ClearProperties) {
  Memory board;
  Entity player = board.createEntity("player");
  ASSERT_TRUE(board.write(player, Vec3f()));
  ASSERT_EQ(board.count<Vec3f>(), 1);
  ASSERT_TRUE(board.remove<Vec3f>());
  ASSERT_EQ(board.count<Vec3f>(), 0);
}

TEST(Memory, Iterate) {
  Memory board;
  board.write(Entity("toto", 1), Vec3f());
  board.write(Entity("toto", 10), Vec3f());
  std::size_t count = 0;
  ASSERT_TRUE(board.process<Vec3f>(
      [&count](Entity const& e, Vec3f const & /*p*/) -> Result {
        if (!e.isValid()) {
          return error("invalid entity: " + e.name());
        }
        ++count;
        return ok();
      }));
  ASSERT_EQ(count, 2);
}

TEST(Memory, Remove) {
  Memory board;
  Entity player = board.createEntity("player");
  ASSERT_FALSE(board.remove<Vec3f>(player));
  ASSERT_EQ(board.count<Vec3f>(), 0);
  ASSERT_TRUE(board.write(player, Vec3f()));
  ASSERT_EQ(board.count<Vec3f>(), 1);
  ASSERT_TRUE(board.remove<Vec3f>(player));
  ASSERT_EQ(board.count<Vec3f>(), 0);
}

TEST(Memory, EasyTwoIterate) {
  Memory board;
  board.write(Entity("toto", 1), Vec3f());
  board.write(Entity("toto", 10), Vec3f());
  board.write(Entity("toto", 1), 0.f);
  board.write(Entity("toto", 10), 0.f);
  std::size_t count = 0;
  Result iterationResult = board.process<Vec3f, float>(
      [&count](Entity const& e, Vec3f const&, float) -> Result {
        if (!e.isValid()) {
          return error("invalid entity: " + e.name());
        }
        ++count;
        return ok();
      });
  ASSERT_TRUE(iterationResult);
  ASSERT_EQ(count, 2);
}

TEST(Memory, TwoIterate) {
  Memory board;
  board.write(Entity("toto", 1), Vec3f());
  board.write(Entity("toto", 10), Vec3f());
  board.write(Entity("toto", 1), 0.f);
  board.write(Entity("toto", 10), 0.f);
  board.write(Entity("toto", 5), 0.f);
  std::size_t count = 0;
  Result iterationResult = board.process<Vec3f, float>(
      [&count](Entity const& e, Vec3f const&, float) -> Result {
        if (!e.isValid()) {
          return error("invalid entity: " + e.name());
        }
        ++count;
        return ok();
      });
  ASSERT_TRUE(iterationResult);
  ASSERT_EQ(count, 2);
}

TEST(Memory, ReadWriteV2) {
  Memory board;
  ASSERT_TRUE(board.write(Vec3f(1.f, 2.f, 3.f)));
  Vec3f read;
  ASSERT_TRUE(board.read(read));
  ASSERT_EQ(read, Vec3f(1.f, 2.f, 3.f));
}
