#include <arty/impl/mouse_system.hpp>
#include <arty/impl/physics_system.hpp>

namespace arty {

Result MouseSystem::process(const Ptr<Memory>& mem,
                            const Ptr<InputManager>& inputs) {
  Camera camera = mem->read<Camera>("camera");
  auto line = camera.raycast(Camera::pixel_type(inputs->position()));
  Entity selected;
  auto closest = std::numeric_limits<float>::max();
  auto data = Vec3f();

  if (mem->count(INPUT_2)) {
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
    if (!mem->process<Tf3f, AABox3f>(INPUT_1, INPUT_2, work)) {
      return error("failed to compute cursor 3d position");
    }
  }

  std::cout << line.origin() << std::endl;
  std::cout << line.direction() << std::endl;

  Selected s;
  s.entity = selected;
  s.point = data;
  if (!selected) {
    auto plano = Geo::intersect(line, Plane3f(Vec3f(), Vec3f(0.f, 0.f, 1.f)));
    if (!plano.empty()) {
      s.point = plano.value();
    }
  }
  std::cout << s.point << std::endl;

  mem->write(OUTPUT, s);

  /*
  static auto toto = mem->createEntity("toto");
  static AABox3f totobox(Vec3f(), Vec3f::all(0.1f));
  if (selected.isValid()) {
    if (mouse->occured(_pop)) {
      auto poped = mem->createEntity("poped");
      Tf3f pose(data);
      pose += Vec3f(0.f, 0.f, 2.f);
      mem->write(poped, PhysicsSystem::INPUT, Physics(pose, 1.f));
      mem->write(poped, "hitbox", AABox3f::unit());
    } else if (mouse->occured(_del)) {
      mem->remove(selected);
    } else {
      mem->write(toto, "transform", Tf3f(data));
      mem->write(toto, "hitbox", totobox);
    }
  } else {
    auto plano = Geo::intersect(line, Plane3f(Vec3f(), Vec3f(0.f, 0.f, 1.f)));
    if (plano.empty()) {
      return res;
    }
    if (mouse->occured(_pop)) {
      auto poped = mem->createEntity("poped");
      Tf3f pose(plano.value());
      pose += Vec3f(0.f, 0.f, 2.f);
      mem->write(poped, PhysicsSystem::INPUT, Physics(pose, 1.f));
      mem->write(poped, "hitbox", AABox3f::unit());
    } else {
      mem->write(toto, "transform", Tf3f(plano.value()));
      mem->write(toto, "sphere", Sphere3f(totobox.center(), 2.f));
    }
  }
  */
  return ok();
}

}  // namespace arty
