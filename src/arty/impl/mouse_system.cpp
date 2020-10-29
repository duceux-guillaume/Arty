#include <arty/impl/mouse_system.hpp>
#include <arty/impl/physics_system.hpp>

namespace arty {

Result MouseSystem::process(const Ptr<Memory>& mem,
                            const Ptr<InputManager>& inputs) {
  Camera camera;
  if (!mem->read<Camera>(camera)) {
    return error("no camera");
  }
  auto line = camera.raycast(Camera::pixel_type(inputs->position()));
  Entity selected;
  auto closest = std::numeric_limits<float>::max();
  auto data = Vec3f();

  if (mem->count<AABox3f>()) {
    auto work = [&](Entity const& e, Tf3f const& t,
                    AABox3f const& b) -> Result {
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
    if (!mem->process<Tf3f, AABox3f>(work)) {
      return error("failed to compute cursor 3d position");
    }
  }

  Selected s;
  s.entity = selected;
  s.point = data;
  if (!selected) {
    auto plano = Geo::intersect(line, Plane3f(Vec3f(), Vec3f(0.f, 0.f, 1.f)));
    if (!plano.empty()) {
      s.point = plano.value();
    }
  }
  mem->write(s);
  return ok();
}

}  // namespace arty
