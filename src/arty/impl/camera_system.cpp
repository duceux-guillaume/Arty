#include <arty/impl/camera_system.hpp>

namespace arty {

static const Mat4x4f origin = {-0.0164683, -0.999487,   -0.0274727, 0,    //
                               0.629497,   -0.0317114,  0.776355,   0,    //
                               -0.776828,  -0.00450899, 0.629695,   -20,  //
                               0,          0,           0,          1};

FixedCameraSystem::FixedCameraSystem(const Ptr<Window>& w)
    : System(),
      _window(w),
      _view(),
      _projection(),
      _camFromWorld(origin),
      _fov(45.0f) {}

Result FixedCameraSystem::process(const Ptr<Memory>& board) {
  float ratio = static_cast<float>(_window->width()) / _window->height();
  if (_window->height() == 0) {
    ratio = 16.f / 9.f;
  }
  Camera camera;
  camera.perspective(radians(_fov), ratio, 0.1f, 100.0f);
  camera.lookAt(Vec3f{0.f, -20.f, 20.f}, Vec3f{0.f, 0.f, 2.f},
                Vec3f{0.f, 1.f, 0.f});
  board->write("camera", camera);

  return ok();
}

Camera::pixel_type Camera::worldToPixel(const Camera::point_type& pt) const {
  vector_type clip = _projection * _inv_rot * _inv_tran *
                     vector_type(pt.x(), pt.y(), pt.z(), number_type(1));
  return pixel_type{clip.x(), clip.y()};
}

void Camera::perspective(const Camera::number_type& fov,
                         const Camera::number_type& aspect,
                         const Camera::number_type& znear,
                         const Camera::number_type& zfar) {
  assert(std::abs(aspect - std::numeric_limits<number_type>::epsilon()) >
         static_cast<number_type>(0));

  number_type const tanHalfFovy = std::tan(fov / number_type(2));
  mat_type mat;
  mat(0, 0) = static_cast<number_type>(1) / (aspect * tanHalfFovy);
  mat(1, 1) = static_cast<number_type>(1) / (tanHalfFovy);
  mat(2, 2) = -(zfar + znear) / (zfar - znear);
  mat(3, 2) = -static_cast<number_type>(1);
  mat(2, 3) = -(static_cast<number_type>(2) * zfar * znear) / (zfar - znear);
  _projection = mat;
}

void Camera::lookAt(const Camera::point_type& eye,
                    const Camera::point_type& target,
                    const Camera::point_type& updir) {
  point_type const forward((eye - target).normalize());
  auto updirn = updir.normalize();
  auto test = updirn.dot(forward);
  point_type right(cross(updirn, forward).normalize());
  if (test == 1) {
    right = cross(point_type(updirn.z(), updirn.x(), updirn.y()), forward)
                .normalize();
  }
  // TODO handle bad up direction
  point_type const up(cross(forward, right));

  _inv_rot = mat_type{
      right.x(),   right.y(),   right.z(),   0,  //
      up.x(),      up.y(),      up.z(),      0,  //
      forward.x(), forward.y(), forward.z(), 0,  //
      0,           0,           0,           1,
  };

  _inv_tran = mat_type{
      1, 0, 0, -eye.x(),  //
      0, 1, 0, -eye.y(),  //
      0, 0, 1, -eye.z(),  //
      0, 0, 0, 1,         //
  };
}

}  // namespace arty
