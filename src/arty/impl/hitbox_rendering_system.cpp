#include <arty/impl/camera_system.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {
Result HitBoxRenderingSystem::process(const Ptr<Memory>& board) {
  auto cam = board->read<Camera>("camera");

  auto work = [=](Entity const& e, Transform const& t,
                  AABox3f const& b) -> Result {
    _renderer->draw(e, b, t.toMat(), cam.view, cam.projection);
    return ok();
  };
  board->process<Transform, AABox3f>("transform", DRAW_PROP, work);
  return ok();
}

Result HitBoxRenderingSystem::init(const Ptr<Memory>& /*board*/) {
  check_result(_renderer->init());
  return ok();
}

IShapeRenderer::~IShapeRenderer() {}

}  // namespace arty
