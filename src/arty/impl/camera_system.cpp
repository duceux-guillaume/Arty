#include <arty/impl/camera_system.hpp>

namespace arty {

static const Mat4x4f origin = {-0.0164683, -0.999487,   -0.0274727, 0,    //
                               0.629497,   -0.0317114,  0.776355,   0,    //
                               -0.776828,  -0.00450899, 0.629695,   -20,  //
                               0,          0,           0,          1};

CameraSystem::CameraSystem(const Ptr<Window> &w)
    : System(),
      _window(w),
      _view(),
      _projection(),
      _camFromWorld(origin),
      _hangle(0.f),
      _vangle(0.f),
      _fov(45.0f),
      _speed(10.f),
      _mouseSpeed(0.002f),
      _mode(CAM_VIEWER) {}

Result CameraSystem::init(const Ptr<Blackboard> &board) {
  _camera_entity = board->createEntity("camera");
  return ok();
}

Result CameraSystem::process(const Ptr<Blackboard> &board) {
  if (_mode == CAM_TPS) {
    auto propptr = board->getProperties<Entity>("camtarget");
    if (!propptr) {
      return error("no camera target given");
    }
    if (propptr->size() != 1) {
      return error("multiple camera target given");
    }
    auto entity = propptr->at(0).value;
    auto *tf = board->getEntityProperty<Transform>(entity, "transform");
    if (!tf) {
      return error("target entity has no transform");
    }
    Mat4x4f target = tf->toMat();
    target = _control.tps(_window, target);
    tf->fromMat(target);
  } else if (_mode == CAM_FPS) {
    _camFromWorld = _control.fps(_window, _camFromWorld);
  } else {
    _camFromWorld = _control.viewer(_window, _camFromWorld);
  }
  CursorPosition cursor(_window->width() / 2, _window->height() / 2);
  _window->setCursorPosition(cursor);

  if (_window->keyHasBeenPressed(Key::TAB)) {
    if (_mode == CAM_FPS) {
      _mode = CAM_VIEWER;
    } else if (_mode == CAM_VIEWER) {
      _mode = CAM_TPS;
    } else {
      _mode = CAM_FPS;
    }
    _camFromWorld = origin;
  }

  float ratio = static_cast<float>(_window->width()) / _window->height();
  if (ratio - std::numeric_limits<float>::epsilon() <= 0.f) {
    ratio = 16.f / 9.f;
  }
  std::cout << "ratio: " << ratio << std::endl;
  _projection = perspective(radians(_fov), ratio, 0.1f, 100.0f);

  Camera camera;
  camera.projection = _projection;
  camera.view = _camFromWorld;
  board->set(_camera_entity, "camera", camera);

  return ok();
}

void CameraSystem::release() {}

Mat4x4f InputControl::tps(Ptr<Window> input, Mat4x4f target) {
  static double lastTime = input->getTime();
  double currentTime = input->getTime();
  float deltaTime = currentTime - lastTime;
  float speed = 10.f;
  if (input->keyHasBeenPressed(Key::UP) || input->keyHasBeenPressed(Key::W)) {
    target = target * translation(deltaTime * speed, 0.f, 0.f);
  }
  if (input->keyHasBeenPressed(Key::DOWN) || input->keyHasBeenPressed(Key::S)) {
    target = target * translation(-deltaTime * speed, 0.f, 0.f);
  }
  if (input->keyHasBeenPressed(Key::LEFT) || input->keyHasBeenPressed(Key::A)) {
    target = target * translation(0.f, deltaTime * speed, 0.f);
  }
  if (input->keyHasBeenPressed(Key::RIGHT) ||
      input->keyHasBeenPressed(Key::D)) {
    target = target * translation(0.f, -deltaTime * speed, 0.f);
  }
  lastTime = currentTime;
  return target;
}

Mat4x4f InputControl::fps(Ptr<Window> input, Mat4x4f target) {
  static double lastTime = input->getTime();
  double currentTime = input->getTime();
  float deltaTime = currentTime - lastTime;

  CursorPosition cursor = input->getCursorPosition();
  float dx = (input->width() / 2.f - cursor.x);
  float dy = (input->height() / 2.f - cursor.y);
  float hangle = 0;
  float vangle = 0;
  float mousespeed = 0.002f;
  float speed = 1.f;
  if (std::fabs(dx) < 100 &&
      std::fabs(dy) < 100) {  // Don't why its not stable at startup
    hangle = mousespeed * (input->width() / 2.f - cursor.x);
    vangle = mousespeed * (input->height() / 2.f - cursor.y);
  }

  if (input->keyHasBeenPressed(Key::UP) || input->keyHasBeenPressed(Key::W)) {
    target = target * translation(0.f, 0.f, deltaTime * speed);
  }
  if (input->keyHasBeenPressed(Key::DOWN) || input->keyHasBeenPressed(Key::S)) {
    target = target * translation(0.f, 0.f, -deltaTime * speed);
  }
  if (input->keyHasBeenPressed(Key::RIGHT) ||
      input->keyHasBeenPressed(Key::A)) {
    target = target * translation(deltaTime * speed, 0.f, 0.f);
  }
  if (input->keyHasBeenPressed(Key::LEFT) || input->keyHasBeenPressed(Key::D)) {
    target = target * translation(-deltaTime * speed, 0.f, 0.f);
  }
  lastTime = currentTime;
  return target * rotation(-vangle, hangle, 0.f);
}

Mat4x4f InputControl::viewer(Ptr<Window> input, Mat4x4f target) {
  static double lastTime = input->getTime();
  double currentTime = input->getTime();
  float deltaTime = currentTime - lastTime;

  CursorPosition cursor = input->getCursorPosition();
  float dx = (input->width() / 2.f - cursor.x);
  float dy = (input->height() / 2.f - cursor.y);
  float hangle = 0;
  float vangle = 0;
  float mousespeed = 0.002f;
  float speed = 1.f;
  if (std::fabs(dx) < 100 &&
      std::fabs(dy) < 100) {  // Don't why its not stable at startup
    hangle = mousespeed * (input->width() / 2.f - cursor.x);
    vangle = mousespeed * (input->height() / 2.f - cursor.y);
  }

  if (input->keyHasBeenPressed(Key::UP) || input->keyHasBeenPressed(Key::W)) {
    target = target * rotation(deltaTime * speed / 2.F, 0.f, 0.f);
  }
  if (input->keyHasBeenPressed(Key::DOWN) || input->keyHasBeenPressed(Key::S)) {
    target = target * rotation(-deltaTime * speed / 2.F, 0.f, 0.f);
  }
  if (input->keyHasBeenPressed(Key::RIGHT) ||
      input->keyHasBeenPressed(Key::A)) {
    target = target * rotation(0.f, 0.f, -deltaTime * speed / 2.F);
  }
  if (input->keyHasBeenPressed(Key::LEFT) || input->keyHasBeenPressed(Key::D)) {
    target = target * rotation(0.f, 0.f, deltaTime * speed / 2.F);
  }
  if (input->keyHasBeenPressed(Key::Q)) {
    auto direction =
        (Vec3f() - Vec3f({target(0, 3), target(1, 3), target(2, 3)}))
            .normalize();
    direction *= (deltaTime * speed);
    target = translation(direction) * target;
  }
  if (input->keyHasBeenPressed(Key::E)) {
    auto direction =
        (Vec3f() - Vec3f({target(0, 3), target(1, 3), target(2, 3)}))
            .normalize();
    direction *= (-deltaTime * speed);
    target = translation(direction) * target;
  }
  if (input->keyHasBeenPressed(Key::Z)) {
    target = target * rotation(0.f, deltaTime * speed / 2.F, 0.f);
  }
  if (input->keyHasBeenPressed(Key::C)) {
    target = target * rotation(0.f, -deltaTime * speed / 2.F, 0.f);
  }
  lastTime = currentTime;
  return target * rotation(-vangle, hangle, 0.f);
}

}  // namespace arty
