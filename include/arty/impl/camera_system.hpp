#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/math.hpp>
#include <arty/core/system.hpp>
#include <arty/core/window.hpp>
#include <cmath>
#include <limits>

namespace arty {

class Camera {
 public:
  using number_type = float;
  using mat_type = Mat<number_type, 4, 4>;
  using point_type = Vec3<number_type>;
  using vector_type = Vec4<number_type>;
  using pixel_type = Vec2<number_type>;

  mat_type const& projection() const { return _projection; }
  mat_type view() const { return _inv_rot * _inv_tran; }
  mat_type position() const { return (-_inv_tran) * _inv_rot.transpose(); }

  pixel_type worldToPixel(point_type const& pt) const;

  void perspective(number_type const& fov, number_type const& aspect,
                   number_type const& znear, number_type const& zfar);

  void lookAt(point_type const& eye, point_type const& target,
              point_type const& updir);

 private:
  Mat4x4f _projection;
  Mat4x4f _inv_rot;
  Mat4x4f _inv_tran;
};

class FixedCameraSystem : public System {
 public:
  FixedCameraSystem(Ptr<Window> const& w);

  Result process(Ptr<Memory> const& board) override;

 private:
  Ptr<Window> _window;
  Mat4x4f _view;
  Mat4x4f _projection;
  Mat4x4f _camFromWorld;
  float _fov;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
