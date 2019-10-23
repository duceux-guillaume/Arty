#ifndef CAMERA_SYSTEM_HPP
#define CAMERA_SYSTEM_HPP

#include <arty/core/ecs.h>
#include <arty/core/math.h>
#include <arty/core/window.h>

#include <cmath>

namespace arty {

class CameraSystem : public System {
 public:
  CameraSystem(Ptr<Window> const& w)
      : System(),
        _window(w),
        _view(),
        _projection(),
        _position({4.f, 3.f, 3.f}),
        _hangle(4.f),
        _vangle(-0.6f),
        _fov(45.0f),
        _speed(10.f),
        _mouseSpeed(0.002f),
        _camera("camera") {}

  Result init(Ptr<Blackboard> const& board) override {
    _window->setCursorPosition(
        CursorPosition(_window->width() / 2.0, _window->height() / 2.0));
    return ok();
  }

  Result process(Ptr<Blackboard> const& board) override {
    static double lastTime = _window->getTime();
    double currentTime = _window->getTime();
    float deltaTime = currentTime - lastTime;

    CursorPosition cursor = _window->getCursorPosition();
    _window->setCursorPosition(
        CursorPosition(_window->width() / 2.0, _window->height() / 2.0));

    float dx = (_window->width() / 2.f - cursor.x);
    float dy = (_window->height() / 2.f - cursor.y);
    if (std::fabs(dx) < 100 &&
        std::fabs(dy) < 100) {  // Don't why its not stable at startup
      _hangle += _mouseSpeed * (_window->width() / 2.f - cursor.x);
      _vangle += _mouseSpeed * (_window->height() / 2.f - cursor.y);
    }
    Vec3f direction{std::cos(_vangle) * std::sin(_hangle),  //
                    std::sin(_vangle),                      //
                    std::cos(_vangle) * std::cos(_hangle)};

    Vec3f right{std::sin(_hangle - 3.14f / 2.0f),  //
                0.f,                               //
                std::cos(_hangle - 3.14f / 2.0f)};

    Vec3f up = cross(right, direction);

    if (_window->keyHasBeenPressed(Key::UP)) {
      _position += direction * deltaTime * _speed;
    }
    if (_window->keyHasBeenPressed(Key::DOWN)) {
      _position -= direction * deltaTime * _speed;
    }
    if (_window->keyHasBeenPressed(Key::RIGHT)) {
      _position += right * deltaTime * _speed;
    }
    if (_window->keyHasBeenPressed(Key::LEFT)) {
      _position -= right * deltaTime * _speed;
    }

    _projection = perspective(radians(_fov), 4.0f / 3.0f, 0.1f, 100.0f);

    _view = lookAt(_position, _position + direction, up);

    Mat4x4f camera = _projection * _view;
    board->set(_camera, "mvp", camera);

    lastTime = currentTime;

    return ok();
  }

  void release() override {}

 private:
  Ptr<Window> _window;
  Mat4x4f _view;
  Mat4x4f _projection;
  Vec3f _position;
  float _hangle;
  float _vangle;
  float _fov;
  float _speed;
  float _mouseSpeed;
  Entity _camera;
};

}  // namespace arty

#endif  // CAMERA_SYSTEM_HPP
