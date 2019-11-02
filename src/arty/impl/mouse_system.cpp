#include <arty/impl/camera_system.hpp>
#include <arty/impl/mouse_system.hpp>

namespace arty {

Result MouseSystem::process(const Ptr<Blackboard> & /*board*/) {
  /*
  CursorPosition cursor = _window->getCursorPosition();
  Vec3f direction(cursor.x, cursor.y, 0.f);
  auto ptr = board->getProperties<Camera>("camera");
  if (!ptr || ptr->size() != 1) {
    return error("error getting camera state");
  }
  Camera cam = ptr->at(0).value;
  Mat4x4f worldcampos = inverse(cam.view);
*/
  return ok();
}

Result MouseSystem::init(const Ptr<Blackboard> & /*board*/) { return ok(); }

}  // namespace arty
