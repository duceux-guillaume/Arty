#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision.hpp>
#include <arty/impl/collision_rendering_system.hpp>

namespace arty {

Result CollisionRenderingSystem::process(const Ptr<Memory>& mem) {
  auto cam = mem->read<Camera>("camera");

  auto work = [=](Entity const& e, std::vector<Collision> const& b) -> Result {
    for (auto const& col : b) {
      _renderer->draw(e, col.shape.pts(), Mat4x4f::identity(), cam.view,
                      cam.projection);
    }
    return ok();
  };
  mem->process<std::vector<Collision>>(INPUT, work);
  return ok();
}

Result CollisionRenderingSystem::init(const Ptr<Memory>&) {
  return _renderer->init();
}

}  // namespace arty
