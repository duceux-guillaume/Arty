#ifndef PLAYER_CONTROL_HPP
#define PLAYER_CONTROL_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/system.hpp>
#include <arty/impl/camera_system.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>
#include <arty/impl/physics_system.hpp>
#include <iostream>

namespace arty {

class PlayerControlSystem : public System {
 private:
  InputEvent _up;

 public:
  // System interface
 public:
  Result process(const Ptr<Memory>& board,
                 Ptr<Keyboard> const& keyboard) override;
  Result init(const Ptr<Memory>& board, Ptr<Keyboard> const& keyboard) override;
  void release() override;
};

class MouseSystem : public System {
 public:
  Result process(Ptr<Memory> const& mem, Ptr<Keyboard> const& /*keyboard*/,
                 Ptr<Mouse> const& mouse) {
    Camera camera = mem->read<Camera>("camera");
    auto mp = mouse->position();
    std::cout << "cursor" << mp << std::endl;
    Vec4f mpp{static_cast<float>(mp.x()), static_cast<float>(mp.y()), -1.f,
              1.f};
    auto position = camera.position();
    auto tmp1 = position * mpp;
    auto tmp2 = Vec3f(position(0, 3), position(1, 3), position(2, 3));
    std::cout << "reprojection" << tmp1 << std::endl;
    Line3f line(tmp2, Vec3f(tmp1.x(), tmp1.y(), tmp1.z()));
    Plane3f xyplane(Vec3f(), Vec3f(1.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f));
    auto toy = Geo3D::intersect(line, xyplane);
    if (toy.exist()) {
      static auto ent = mem->createEntity("toy");
      std::cout << "toy" << toy.value() << std::endl;
      mem->write(ent, "transform", Tf3f(toy.value()));
      mem->write(ent, "hitbox", AABox3f(Vec3f::zero(), Vec3f::all(1.f)));
    }
    return ok();
  }
};

}  // namespace arty

#endif  // PLAYER_CONTROL_HPP
