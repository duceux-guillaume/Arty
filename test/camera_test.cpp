#include <gtest/gtest.h>

#include <arty/impl/camera_system.hpp>

using namespace arty;

using pt_t = Camera::point_type;
using vec_t = Camera::vector_type;
using pix_t = Camera::pixel_type;

TEST(Camera, LookAt) {
  Camera camera;
  camera.perspective(radians(45.f), 16.f / 9.f, 0.1f, 100.0f);
  {
    camera.lookAt(pt_t{0.f, 10.f, 0.f}, pt_t{0.f, 0.f, 0.f},
                  pt_t{0.f, 0.f, 1.f});
    auto view_space = camera.view() * vec_t{0.f, 0.f, 0.f, 1.f};
    ASSERT_EQ(view_space, vec_t(0.f, 0.f, -10.f, 1.f));
  }
  {
    camera.lookAt(pt_t{0.f, -10.f, 0.f}, pt_t{0.f, 0.f, 0.f},
                  pt_t{0.f, 0.f, 1.f});
    auto view_space = camera.view() * vec_t{0.f, 0.f, 0.f, 1.f};
    ASSERT_EQ(view_space, vec_t(0.f, 0.f, -10.f, 1.f));
  }
  {
    camera.lookAt(pt_t{0.f, 0.f, 10.f}, pt_t{0.f, 0.f, 0.f},
                  pt_t{0.f, 1.f, 0.f});
    auto view_space = camera.view() * vec_t{0.f, 0.f, 0.f, 1.f};
    ASSERT_EQ(view_space, vec_t(0.f, 0.f, -10.f, 1.f));
  }
  {
    camera.lookAt(pt_t{0.f, 0.f, -10.f}, pt_t{0.f, 0.f, 0.f},
                  pt_t{0.f, 10.f, 0.f});
    auto view_space = camera.view() * vec_t{0.f, 0.f, 0.f, 1.f};
    ASSERT_EQ(view_space, vec_t(0.f, 0.f, -10.f, 1.f));
  }
  {
    camera.lookAt(pt_t{10.f, 0.f, 0.f}, pt_t{0.f, 0.f, 0.f},
                  pt_t{0.f, 1.f, 0.f});
    auto view_space = camera.view() * vec_t{0.f, 0.f, 0.f, 1.f};
    ASSERT_EQ(view_space, vec_t(0.f, 0.f, -10.f, 1.f));
  }
  {
    camera.lookAt(pt_t{-10.f, 0.f, 0.f}, pt_t{0.f, 0.f, 0.f},
                  pt_t{0.f, 1.f, 0.f});
    auto view_space = camera.view() * vec_t{0.f, 0.f, 0.f, 1.f};
    ASSERT_EQ(view_space, vec_t(0.f, 0.f, -10.f, 1.f));
  }
}

TEST(Camera, worldToPixel) {
  Camera camera;
  camera.perspective(radians(45.f), 16.f / 9.f, 0.1f, 100.0f);
  camera.lookAt(pt_t{10.f, 0.f, 0.f}, pt_t{0.f, 0.f, 0.f}, pt_t{0.f, 1.f, 0.f});
  auto clip_space = camera.worldToPixel(pt_t{0.f, 0.f, 0.f});
  ASSERT_EQ(clip_space, pix_t(0.f, 0.f));
}

TEST(Camera, badLookAt) {
  Camera camera;
  camera.perspective(radians(45.f), 16.f / 9.f, 0.1f, 100.0f);
  camera.lookAt(pt_t{10.f, 0.f, 0.f}, pt_t{0.f, 0.f, 0.f}, pt_t{1.f, 0.f, 0.f});
  auto clip_space = camera.worldToPixel(pt_t{0.f, 0.f, 0.f});
  ASSERT_EQ(clip_space, pix_t(0.f, 0.f));
}
TEST(Camera, position) {
  Camera camera;
  camera.perspective(radians(45.f), 16.f / 9.f, 0.1f, 100.0f);
  camera.lookAt(pt_t{10.f, 0.f, 0.f}, pt_t{0.f, 0.f, 0.f}, pt_t{1.f, 0.f, 0.f});
  auto camera_pos = camera.position().block<float, 3, 1>(0, 3);
  ASSERT_EQ(camera_pos, pt_t(10.f, 0.f, 0.f));
}
