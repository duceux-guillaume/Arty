#include <arty/impl/camera_system.hpp>

namespace arty {

static const Mat4x4f origin = {-0.0164683, -0.999487,   -0.0274727, 0,    //
                               0.629497,   -0.0317114,  0.776355,   0,    //
                               -0.776828,  -0.00450899, 0.629695,   -20,  //
                               0,          0,           0,          1};

FixedCameraSystem::FixedCameraSystem(const Ptr<Window> &w)
    : System(),
      _window(w),
      _view(),
      _projection(),
      _camFromWorld(origin),
      _fov(45.0f) {}

Result FixedCameraSystem::init(const Ptr<Memory> &board) {
  _camera_entity = board->createEntity("camera");
  return ok();
}

Result FixedCameraSystem::process(const Ptr<Memory> &board) {
  float ratio = static_cast<float>(_window->width()) / _window->height();
  if (_window->height() == 0) {
    ratio = 16.f / 9.f;
  }
  _projection = perspective(radians(_fov), ratio, 0.1f, 100.0f);

  Camera camera;
  camera.projection = _projection;
  camera.view = _camFromWorld;
  board->set(_camera_entity, "camera", camera);

  return ok();
}

void FixedCameraSystem::release() {}

}  // namespace arty
