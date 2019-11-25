#include <gtest/gtest.h>

#include <arty/core/memory.hpp>

using namespace arty;

TEST(Memory, newEnt) {
  Memory mem;
  ASSERT_TRUE(mem.newEnt("camera"));
  ASSERT_FALSE(mem.newEnt("camera"));
}

TEST(Memory, hasEnt) {
  Memory mem;
  ASSERT_FALSE(mem.hasEnt("camera"));
  ASSERT_TRUE(mem.newEnt("camera"));
  ASSERT_TRUE(mem.hasEnt("camera"));
}

TEST(Memory, newProp) {
  Memory mem;
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_FALSE(mem.newProp("physics"));
}

TEST(Memory, hasProp) {
  Memory mem;
  ASSERT_FALSE(mem.hasProp("physics"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.hasProp("physics"));
}

TEST(Memory, set) {
  Memory mem;
  ASSERT_FALSE(mem.set("bot", "physics", "whatever"));
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.set("bot", "physics", 0));
  ASSERT_TRUE(mem.set("bot", "physics", 1));
  ASSERT_FALSE(mem.set("bot", "physics", "no type change please"));
}

TEST(Memory, get) {
  Memory mem;
  ASSERT_FALSE(mem.get<std::string>("bot", "physics"));
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.set("bot", "physics", 10));
  ASSERT_FALSE(mem.get<double>("bot", "physics"));
  ASSERT_TRUE(mem.get<int>("bot", "physics"));
}

TEST(Memory, remove) {
  Memory mem;
  ASSERT_FALSE(mem.remove("bot", "physics"));
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.set("bot", "physics", "whatever"));
  ASSERT_TRUE(mem.get<std::string>("bot", "physics"));
  ASSERT_TRUE(mem.remove("bot", "physics"));
  ASSERT_FALSE(mem.remove("bot", "physics"));
}

TEST(Memory, rmEnt) {
  Memory mem;
  ASSERT_FALSE(mem.rmEnt("bot"));
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.set("bot", "physics", "whatever"));
  ASSERT_TRUE(mem.rmEnt("bot"));
  ASSERT_FALSE(mem.rmEnt("bot"));
  ASSERT_FALSE(mem.get<std::string>("bot", "physics"));
}

TEST(Memory, rmProp) {
  Memory mem;
  ASSERT_FALSE(mem.rmProp("physics"));
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.set("bot", "physics", "whatever"));
  ASSERT_TRUE(mem.rmProp("physics"));
  ASSERT_FALSE(mem.rmProp("physics"));
  ASSERT_FALSE(mem.get<std::string>("bot", "physics"));
}

TEST(Memory, IterateOverOneProp) {
  Memory mem;
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("physics"));
  ASSERT_TRUE(mem.set("bot", "physics", 10));
  auto it = mem.iterator<int>("physics");
  for (auto& p : it) {
    ASSERT_EQ(p.first, "bot");
    ASSERT_EQ(p.second, 10);
    p.second = 15;
  }
  ASSERT_EQ(*mem.get<int>("bot", "physics"), 15);
}
/*
TEST(Memory, IterateOverTwoProps) {
  Memory mem;
  ASSERT_TRUE(mem.newEnt("bot"));
  ASSERT_TRUE(mem.newProp("health"));
  ASSERT_TRUE(mem.newProp("name"));
  ASSERT_TRUE(mem.set("bot", "health", 10));
  ASSERT_TRUE(mem.set("bot", "name", std::string("myname")));
  auto wrapper = mem.props("health", "name");
  ASSERT_TRUE(wrapper);
  for (auto const& p : wrapper) {
    ASSERT_EQ(p[0]->key(), "bot");
    ASSERT_EQ(*p[0], 10);
    ASSERT_EQ(p[1]->key(), "bot");
    ASSERT_EQ(*p[1], std::string("myname"));
  }
}
TEST(Memory, IterateOverTwoPropsBadLuck) {
  Memory mem;
  ASSERT_TRUE(mem.newEnt("bot1"));
  ASSERT_TRUE(mem.newProp("health"));
  ASSERT_TRUE(mem.newEnt("bot2"));
  ASSERT_TRUE(mem.newProp("name"));
  ASSERT_TRUE(mem.set("bot1", "health", 10));
  ASSERT_TRUE(mem.set("bot2", "name", std::string("myname")));
  auto wrapper = mem.props("health", "name");
  ASSERT_TRUE(wrapper);
  for (auto const& p : wrapper) {
    // no entity has both those props
    // we shouldn't iterate at all
    FAIL();
    (void)p;
  }
}

TEST(Memory, IterateOverTwoPropsSkipOne) {
  Memory mem;
  ASSERT_TRUE(mem.newEnt("bot1"));
  ASSERT_TRUE(mem.newProp("health"));
  ASSERT_TRUE(mem.newEnt("bot2"));
  ASSERT_TRUE(mem.newProp("name"));
  ASSERT_TRUE(mem.set("bot1", "health", 10));
  ASSERT_TRUE(mem.set("bot2", "health", 10));
  ASSERT_TRUE(mem.set("bot2", "name", std::string("myname")));
  auto wrapper = mem.props("health", "name");
  ASSERT_TRUE(wrapper);
  for (auto const& p : wrapper) {
    // bot 1 should be skipped it is missing a prop
    ASSERT_EQ(p[0]->key(), "bot2");
    ASSERT_EQ(*p[0], 10);
    ASSERT_EQ(p[1]->key(), "bot2");
    ASSERT_EQ(*p[1], std::string("myname"));
  }
}

TEST(Memory, Write) {
  Memory mem;
  ASSERT_TRUE(mem.newEnt("bot1"));
  ASSERT_TRUE(mem.newProp("health"));
  ASSERT_TRUE(mem.newEnt("bot2"));
  ASSERT_TRUE(mem.newProp("name"));
  ASSERT_TRUE(mem.set("bot1", "health", 10));
  ASSERT_TRUE(mem.set("bot2", "health", 10));
  ASSERT_TRUE(mem.set("bot2", "name", std::string("myname")));
  std::stringstream ss;
  mem.write(ss);
  ASSERT_EQ(ss.str(), "name bot2:myname \nhealth bot1:10 bot2:10 \n");
}
*/
