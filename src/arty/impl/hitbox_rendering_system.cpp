#include <arty/impl/camera_system.hpp>
#include <arty/impl/hitbox_rendering_system.hpp>

namespace arty {
Result HitBoxRenderingSystem::process(const Ptr<Memory>& board) {
  Camera cam;
  if (!board->read<Camera>(cam)) {
    return error("no camera");
  }

  if (board->count<AABox3f>()) {  // AABB
    auto work = [=](Entity const& e, Tf3f const& t,
                    AABox3f const& b) -> Result {
      _renderer->draw(e, b, t.toMat(), cam.view(), cam.projection());
      return ok();
    };
    board->process<Tf3f, AABox3f>(work);
  }
  if (board->count<OBB3f>()) {  // OBB
    auto work = [=](Entity const& e, Tf3f const& t, OBB3f const& b) -> Result {
      _renderer->draw(e, b, t.toMat(), cam.view(), cam.projection());
      return ok();
    };
    board->process<Tf3f, OBB3f>(work);
  }
  if (board->count<Sphere3f>()) {  // Sphere
    auto work = [=](Entity const& e, Tf3f const& t,
                    Sphere3f const& b) -> Result {
      _renderer->draw(e, b, t.toMat(), cam.view(), cam.projection());
      return ok();
    };
    board->process<Tf3f, Sphere3f>(work);
  }

  return ok();
}

Result HitBoxRenderingSystem::init(const Ptr<Memory>& /*board*/) {
  check_result(_renderer->init());
  return ok();
}

IShapeRenderer::~IShapeRenderer() {}

}  // namespace arty
