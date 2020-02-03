#include <gtest/gtest.h>

#include <arty/impl/physics.hpp>

using namespace arty;

TEST(Physics, detectCollision) {
  {
    Tf3f tf1;
    Tf3f tf2(Vec3f{1.5f, 0.f, 0.f});
    AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
    Physics detector;
    auto collision = detector.detectCollision(tf1, box, tf2, box);
    ASSERT_TRUE(collision.exist());
    ASSERT_EQ(collision.penetration(), 0.5);
    ASSERT_EQ(collision.normal(), vector_t(-1, -0, -0));
    ASSERT_EQ(collision.center(), vector_t(0.75, 0., 0.));
  }
  {
    Tf3f tf1;
    Tf3f tf2(Vec3f{0.f, 0.f, 2.f});
    AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
    Physics detector;
    auto collision = detector.detectCollision(tf1, box, tf2, box);
    ASSERT_TRUE(collision.exist());
    ASSERT_EQ(collision.penetration(), 0.);
    ASSERT_EQ(collision.normal(), vector_t(0, 0, -1));
    ASSERT_EQ(collision.center(), vector_t(0, 0, 1));
  }
}

TEST(Physics, integrateMotion) {
  Particle p;
  Physics phy;
  phy.integrateMotion(p, 1.);
  ASSERT_EQ(p.position, vector_t(0, 0, 0));
  ASSERT_EQ(p.velocity, vector_t(0, 0, -10));
  phy.integrateMotion(p, 1.);
  ASSERT_EQ(p.position, vector_t(0, 0, -10));
  ASSERT_EQ(p.velocity, vector_t(0, 0, -19));
}

TEST(Physics, resolveVelocity) {
  {  // no speed penetrating boxes
    Particle p1, p2;
    p2.position = vector_t(1.5f, 0.f, 0.f);
    AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
    Physics detector;
    auto collision =
        detector.detectCollision(p1.transform(), box, p2.transform(), box);
    ASSERT_TRUE(collision.exist());
    ASSERT_EQ(detector.separatingVelocity(collision, p1, p2), 0);
    detector.resolveVelocity(collision, p1, p2, 1);
    ASSERT_EQ(p1.position, vector_t());
    ASSERT_EQ(p1.velocity, vector_t());
    ASSERT_EQ(p2.position, vector_t(1.5f, 0.f, 0.f));
    ASSERT_EQ(p2.velocity, vector_t());
  }
  {  // 1 bump into 2 penetrating boxes
    Particle p1, p2;
    p1.velocity = vector_t(1., 0., 0.);
    p2.position = vector_t(1.5f, 0.f, 0.f);
    AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
    Physics detector;
    auto collision =
        detector.detectCollision(p1.transform(), box, p2.transform(), box);
    ASSERT_TRUE(collision.exist());
    ASSERT_EQ(detector.separatingVelocity(collision, p1, p2), -1);
    detector.resolveVelocity(collision, p1, p2, 1);
    ASSERT_EQ(p1.position, vector_t());
    ASSERT_EQ(p1.velocity, vector_t(0.4, 0., 0.));
    ASSERT_EQ(p2.position, vector_t(1.5f, 0.f, 0.f));
    ASSERT_EQ(p2.velocity, vector_t(0.6, 0., 0.));
  }
  {  // 2 on top of 1
    Particle p1, p2;
    p1.setMass(0);
    p2.position = vector_t(0, 0, 2);
    AABox3f box(Vec3f::zero(), Vec3f::all(1.f));
    Physics detector;
    auto collision =
        detector.detectCollision(p1.transform(), box, p2.transform(), box);
    ASSERT_TRUE(collision.exist());
    ASSERT_EQ(detector.separatingVelocity(collision, p1, p2), 0);
    detector.resolveVelocity(collision, p1, p2, 1);
    ASSERT_EQ(p1.position, vector_t());
    ASSERT_EQ(p1.velocity, vector_t());
    ASSERT_EQ(p2.position, vector_t(0, 0, 2));
    ASSERT_EQ(p2.velocity, vector_t());
    detector.integrateMotion(p2, 0.1);
    ASSERT_EQ(detector.separatingVelocity(collision, p1, p2), -1);
    ASSERT_EQ(p2.velocity, vector_t(0, 0, -1));
    detector.resolveVelocity(collision, p1, p2, 1);
    ASSERT_EQ(p1.position, vector_t());
    ASSERT_EQ(p1.velocity, vector_t());
    ASSERT_EQ(p2.position, vector_t(0, 0, 2));
    ASSERT_EQ(p2.velocity, vector_t());
  }
}
