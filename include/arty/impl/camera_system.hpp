#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/math.h>
#include <arty/core/window.h>

#include <arty/core/system.hpp>
#include <cmath>

namespace arty {

enum CameraMode {
  CAM_FPS,
  CAM_VIEWER,
  CAM_TPS,
};

struct Camera {
  Mat4x4f projection;
  Mat4x4f view;
};

class InputControl {
 public:
  Mat4x4f tps(Ptr<Window> input, Mat4x4f target);
  Mat4x4f fps(Ptr<Window> input, Mat4x4f target);
  Mat4x4f viewer(Ptr<Window> input, Mat4x4f target);

 private:
};

class CameraSystem : public System {
 public:
  CameraSystem(Ptr<Window> const& w);

  Result init(Ptr<Blackboard> const& board) override;

  Result process(Ptr<Blackboard> const& board) override;

  void release() override;

 private:
  Ptr<Window> _window;
  Mat4x4f _view;
  Mat4x4f _projection;
  Mat4x4f _camFromWorld;
  Mat4x4f _target;
  float _hangle;
  float _vangle;
  float _fov;
  float _speed;
  float _mouseSpeed;
  CameraMode _mode;
  Entity _camera_entity;
  InputControl _control;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
