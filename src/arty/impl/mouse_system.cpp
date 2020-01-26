#include <arty/impl/mouse_system.hpp>

namespace arty {

Result MouseSystem::process(const Ptr<Memory>& mem, const Ptr<Keyboard>&,
                            const Ptr<Mouse>& mouse) {
  Camera camera = mem->read<Camera>("camera");
  auto line = camera.raycast(Camera::pixel_type(mouse->position()));
  auto selected = Entity("", 0);
  auto closest = std::numeric_limits<float>::max();
  auto data = Vec3f();

  auto work = [&](Entity const& e, Tf3f const& t, AABox3f const& b) -> Result {
    auto box = b.move(t);
    auto intersection = Geo::intersect(line, box);
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

  static auto toto = mem->createEntity("toto");
  static AABox3f totobox(Vec3f(), Vec3f::all(0.1f));
  if (selected.isValid()) {
    mem->write(selected, OUTPUT, data);
    mem->write(toto, "transform", Tf3f(data));
    mem->write(toto, "hitbox", totobox);
  } else {
    auto plano = Geo::intersect(line, Plane3f(Vec3f(), Vec3f(0.f, 0.f, 1.f)));
    if (plano.empty()) {
      return res;
    }
    mem->write(toto, "transform", Tf3f(plano.value()));
    mem->write(toto, "hitbox", totobox);
  }
  return res;
}

}  // namespace arty
