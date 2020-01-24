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
    Vec4f mpp{static_cast<float>(mp.x()), static_cast<float>(mp.y()), 0.f, 0.f};
    auto tmp1 = camera.view().inv() * mpp;
    std::cout << "reprojection" << tmp1 << std::endl;

    //    static bool first_time = true;
    //    if (first_time) {
    //      mem->clear();
    //      first_time = false;
    //    }
    static auto entity = mem->createEntity("toy");
    Tf3f pos(Vec3f(tmp1.x(), tmp1.y(), 0.f));
    //    mem->write(entity, PhysicsSystem::INPUT, Physics(pos, 0.f));
    //    mem->write(entity, HitBoxRenderingSystem::DRAW_PROP,
    //               AABox3f(Vec3f::zero(), Vec3f::all(0.1f)));
    return ok();
  }
};

}  // namespace arty

#endif  // PLAYER_CONTROL_HPP
