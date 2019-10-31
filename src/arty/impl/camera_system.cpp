#include <arty/impl/camera_system.hpp>

namespace arty {

CameraSystem::CameraSystem(const Ptr<Window> &w)
    : System(),
      _window(w),
      _view(),
      _projection(),
      _camFromWorld(translation(0.f, 0.f, -10.f)),
      _hangle(4.f),
      _vangle(-0.6f),
      _fov(45.0f),
      _speed(10.f),
      _mouseSpeed(0.002f),
      _mode(0) {}

Result CameraSystem::init(const Ptr<Blackboard> &board) {
  _window->setCursorPosition(
      CursorPosition(_window->width() / 2.0, _window->height() / 2.0));
  _camera_entity = board->createEntity("camera");
  return ok();
}

Result CameraSystem::process(const Ptr<Blackboard> &board) {
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
    _hangle = _mouseSpeed * (_window->width() / 2.f - cursor.x);
    _vangle = _mouseSpeed * (_window->height() / 2.f - cursor.y);
  } else {
    _hangle = 0;
    _vangle = 0;
  }

  // FIXME: Key event suppose you have a qwerty
  if (_window->keyHasBeenPressed(Key::UP) ||
      _window->keyHasBeenPressed(Key::W)) {
    if (_mode == 0) {
      _camFromWorld = _camFromWorld * translation(0.f, 0.f, deltaTime * _speed);
    } else {
      _camFromWorld =
          _camFromWorld * rotation(deltaTime * _speed / 2.F, 0.f, 0.f);
    }
  }
  if (_window->keyHasBeenPressed(Key::DOWN) ||
      _window->keyHasBeenPressed(Key::S)) {
    if (_mode == 0) {
      _camFromWorld =
          _camFromWorld * translation(0.f, 0.f, -deltaTime * _speed);
    } else {
      _camFromWorld =
          _camFromWorld * rotation(-deltaTime * _speed / 2.F, 0.f, 0.f);
    }
  }
  if (_window->keyHasBeenPressed(Key::RIGHT) ||
      _window->keyHasBeenPressed(Key::A)) {
    if (_mode == 0) {
      _camFromWorld = _camFromWorld * translation(deltaTime * _speed, 0.f, 0.f);
    } else {
      _camFromWorld =
          _camFromWorld * rotation(0.f, 0.f, -deltaTime * _speed / 2.F);
    }
  }
  if (_window->keyHasBeenPressed(Key::LEFT) ||
      _window->keyHasBeenPressed(Key::D)) {
    if (_mode == 0) {
      _camFromWorld =
          _camFromWorld * translation(-deltaTime * _speed, 0.f, 0.f);
    } else {
      _camFromWorld =
          _camFromWorld * rotation(0.f, 0.f, deltaTime * _speed / 2.F);
    }
  }
  if (_window->keyHasBeenPressed(Key::TAB)) {
    _mode = !_mode;
    _camFromWorld = translation(0.f, 0.f, -10.f);
  }
  if (_window->keyHasBeenPressed(Key::Q)) {
    if (_mode == 1) {
      auto direction =
          normalize(Vec3f() - Vec3f({_camFromWorld(0, 3), _camFromWorld(1, 3),
                                     _camFromWorld(2, 3)}));
      direction *= (deltaTime * _speed);
      _camFromWorld = translation(direction) * _camFromWorld;
    }
  }
  if (_window->keyHasBeenPressed(Key::E)) {
    if (_mode == 1) {
      auto direction =
          normalize(Vec3f() - Vec3f({_camFromWorld(0, 3), _camFromWorld(1, 3),
                                     _camFromWorld(2, 3)}));
      direction *= (-deltaTime * _speed);
      _camFromWorld = translation(direction) * _camFromWorld;
    }
  }
  if (_window->keyHasBeenPressed(Key::Z)) {
    if (_mode == 1) {
      _camFromWorld =
          _camFromWorld * rotation(0.f, deltaTime * _speed / 2.F, 0.f);
    }
  }
  if (_window->keyHasBeenPressed(Key::C)) {
    if (_mode == 1) {
      _camFromWorld =
          _camFromWorld * rotation(0.f, -deltaTime * _speed / 2.F, 0.f);
    }
  }

  _camFromWorld = _camFromWorld * rotation(-_vangle, _hangle, 0.f);

  _projection = perspective(radians(_fov), 4.0f / 3.0f, 0.1f, 100.0f);

  Camera camera;
  camera.projection = _projection;
  camera.view = _camFromWorld;
  board->set(_camera_entity, "camera", camera);

  lastTime = currentTime;

  return ok();
}

void CameraSystem::release() {}

}  // namespace arty
