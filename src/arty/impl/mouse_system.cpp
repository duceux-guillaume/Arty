#include <arty/impl/mouse_system.hpp>
#include <arty/impl/physics_system.hpp>

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
  return res;
}

Result MouseSystem::init(const Ptr<Memory>&, const Ptr<Keyboard>&,
                         const Ptr<Mouse>& mouse) {
  if (!mouse->registerEvent(Mouse::LEFT, InputDevice::Action::PRESS, _pop)) {
    return error("couldn't register event");
  }
  if (!mouse->registerEvent(Mouse::RIGHT, InputDevice::Action::PRESS, _del)) {
    return error("couldn't register event");
  }
  return ok();
}

}  // namespace arty
