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
  ASSERT_TRUE(mem.write(mem.createEntity("toto"), "hp", 10));
}

TEST(Memory, SetGet) {
  Memory mem;
  Entity world = mem.createEntity("world");
  mem.write(world, "position", Vec3f());
  Entity player = mem.createEntity("player");
  mem.write(player, "position", Vec3f({1, 2, 3}));
  ASSERT_EQ(mem.count("position"), 2);
  auto ptr2 = mem.read<Vec3f>(player, "position");
  ASSERT_EQ(ptr2, Vec3f({1, 2, 3}));
}

TEST(Memory, SetGet2) {
  Memory board;
  Entity world = board.createEntity("world");
  board.write(world, "position", Vec3f());
  ASSERT_EQ(board.read<Vec3f>(world, "position"), Vec3f());
  board.write(world, "position", Vec3f(1.f));
  ASSERT_EQ(board.read<Vec3f>(world, "position"), Vec3f(1.f));
}

TEST(Memory, ClearProperties) {
  Memory board;
  Entity player = board.createEntity("player");
  board.write(player, "position", Vec3f());
  ASSERT_EQ(board.count("position"), 1);
  ASSERT_TRUE(board.remove("position"));
  ASSERT_EQ(board.count("position"), 0);
}

TEST(Memory, Iterate) {
  Memory board;
  board.write(Entity("toto", 1), "position", Vec3f());
  board.write(Entity("toto", 10), "position", Vec3f());
  std::size_t count = 0;
  ASSERT_TRUE(board.process<Vec3f>(
      "position", [&count](Entity const& e, Vec3f const & /*p*/) -> Result {
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
  ASSERT_FALSE(board.remove(player, "position"));
  ASSERT_EQ(board.count("position"), 0);
  ASSERT_TRUE(board.write(player, "position", Vec3f()));
  ASSERT_EQ(board.count("position"), 1);
  ASSERT_TRUE(board.remove(player, "position"));
  ASSERT_EQ(board.count("position"), 0);
}

TEST(Memory, EasyTwoIterate) {
  Memory board;
  board.write(Entity("toto", 1), "position", Vec3f());
  board.write(Entity("toto", 10), "position", Vec3f());
  board.write(Entity("toto", 1), "speed", 0.f);
  board.write(Entity("toto", 10), "speed", 0.f);
  std::size_t count = 0;
  Result iterationResult = board.process<Vec3f, float>(
      "position", "speed",
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
  board.write(Entity("toto", 1), "position", Vec3f());
  board.write(Entity("toto", 10), "position", Vec3f());
  board.write(Entity("toto", 1), "speed", 0.f);
  board.write(Entity("toto", 10), "speed", 0.f);
  board.write(Entity("toto", 5), "speed", 0.f);
  std::size_t count = 0;
  Result iterationResult = board.process<Vec3f, float>(
      "position", "speed",
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

TEST(Memory, ReadWrite2V2) {
  Memory board;
  ASSERT_TRUE(board.write(Entity("toto", 1), Vec3f(1.f, 2.f, 3.f)));
  Vec3f read;
  ASSERT_TRUE(board.read(Entity("toto", 1), read));
  ASSERT_EQ(read, Vec3f(1.f, 2.f, 3.f));
}
