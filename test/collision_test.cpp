#include <gtest/gtest.h>

#include <arty/impl/collision.hpp>
#include <arty/impl/mesh_loader_system.hpp>

using namespace arty;

TEST(CollisionDetection, detect) {
  Tf3f tf1;
  Tf3f tf2(Vec3f{1.5f, 0.f, 0.f});
  AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
  CollisionDetection detector;
  auto collision = detector.detect(tf1, box, tf2, box);
  ASSERT_TRUE(collision.exist());
}
/*
TEST(CollisionSolver, detectAndResolve) {
  auto obj1 = makeBrick("floor", Tf3f(), Vec3f::all(1.f), 0.f);
  auto obj2 = makeBrick("paf", Vec3f{1.5f, 0.f, 0.f}, Vec3f::all(1.f), 1.f);
  CollisionDetection detector;
  auto collision =
      detector.detect(obj1.p.position(), obj1.b, obj2.p.position(), obj2.b);
  ASSERT_TRUE(collision.exist());
  CollisionSolver solver;
  solver.solve(collision, &obj1.p, obj2.p);
  solver.solve(collision, &obj2.p, obj1.p);
  collision =
      detector.detect(obj1.p.position(), obj1.b, obj2.p.position(), obj2.b);
  ASSERT_EQ(obj1.p.position().translation(), Vec3f::zero());
  ASSERT_EQ(obj2.p.position().translation(), Vec3f(2.f, 0.f, 0.f));
  ASSERT_TRUE(collision.exist());
  ASSERT_FALSE(collision.hasVolume());
}
*/
