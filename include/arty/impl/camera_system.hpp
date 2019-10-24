#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/ecs.h>
#include <arty/core/math.h>
#include <arty/core/window.h>

#include <cmath>

namespace arty {

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
  Entity _camera;
  int _mode;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
