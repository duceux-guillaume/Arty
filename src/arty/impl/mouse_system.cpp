#include <arty/impl/mouse_system.hpp>

namespace arty {

Result MouseSystem::process(const Ptr<Memory>& mem, const Ptr<Keyboard>&,
                            const Ptr<Mouse>& mouse) {
  Camera camera = mem->read<Camera>("camera");
  auto line = camera.raycast(Camera::pixel_type(mouse->position()));
  auto selected = Entity("root", 0);
  auto closest = std::numeric_limits<float>::max();
  auto data = Vec3f();

  auto work = [&](Entity const& e, Tf3f const& t, AABox3f const& b) -> Result {
    auto box = b.move(t);
    auto intersection = Geo3D::intersect(line, box);
    if (intersection.exist()) {
      /* if closer to camera, select this one */
      auto dist = (line.origin() - intersection.value()).normsqr();
      if (dist < closest) {
        selected = e;
        closest = dist;
        data = intersection.value();
      }
    }
    return ok();
  };
  auto res = mem->process<Tf3f, AABox3f>(INPUT_1, INPUT_2, work);

  if (selected.isValid()) {
    mem->write(selected, OUTPUT, data);
  }
  return res;
}

}  // namespace arty
