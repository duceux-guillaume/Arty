#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/math.h>
#include <arty/core/window.h>

#include <arty/core/system.hpp>

namespace arty {

struct Camera {
  Mat4x4f projection;
  Mat4x4f view;
};

class FixedCameraSystem : public System {
 public:
  FixedCameraSystem(Ptr<Window> const& w);

  Result init(Ptr<Blackboard> const& board) override;

  Result process(Ptr<Blackboard> const& board) override;

  void release() override;

 private:
  Ptr<Window> _window;
  Mat4x4f _view;
  Mat4x4f _projection;
  Mat4x4f _camFromWorld;
  float _fov;
  Entity _camera_entity;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
