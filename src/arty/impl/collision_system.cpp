#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_system.hpp>

namespace arty {

Result CollisionDetectionSystem::process(Ptr<Memory> const& mem) {
  mem->remove<CollisionArray>();
  auto first_loop = [mem, this](Entity const& e, Tf3f const& t,
                                AABox3f const& b) -> Result {
    auto second_loop = [mem, this, e, t, b](Entity const& e2, Tf3f const& t2,
                                            AABox3f const& b2) -> Result {
      if (e >= e2) {
        return ok();
      }

      Collision col = _collision.detect(t, b, t2, b2);
      if (col.exist()) {
        col.set(e, e2);
        CollisionArray cols1, cols2;
        // no previous collision is fine
        (void)mem->read<CollisionArray>(e, cols1);
        cols1.push_back(col);
        mem->write(e, cols1);
        (void)mem->read<CollisionArray>(e2, cols2);
        cols2.push_back(col);
        mem->write(e2, cols2);
      }
      return ok();
    };
    return mem->process<Tf3f, AABox3f>(second_loop);
  };
  return mem->process<Tf3f, AABox3f>(first_loop);
}  // namespace arty

Result CollisionRenderingSystem::process(const Ptr<Memory>& mem) {
  Camera cam;
  if (!mem->read<Camera>(cam)) {
    return error("no camera provided");
  }

  auto work = [&](Entity const& e, std::vector<Collision> const& b) -> Result {
    for (auto const& col : b) {
      _renderer->draw(e, col.intersection(), Mat4x4f::identity(), cam.view(),
                      cam.projection());
    }
    return ok();
  };
  if (mem->count<CollisionArray>() > 0) {
    return mem->process<CollisionArray>(work);
  }
  return ok();
}

Result CollisionRenderingSystem::init(const Ptr<Memory>&) {
  return _renderer->init();
}

Result CollisionSolverSystem::process(Ptr<Memory> const& mem) {
  auto work = [&](Entity const& e, std::vector<Collision> const& b) -> Result {
    for (auto const& col : b) {
      Physics phy1, phy2;
      if (!mem->read<Physics>(col.entities().first, phy1)) {
        continue;
      }
      if (!mem->read<Physics>(col.entities().second, phy2)) {
        continue;
      }
      if (e == col.entities().first) {
        _solver.solve(col, &phy1, phy2);
        mem->write(e, phy1);
      } else {
        _solver.solve(col, &phy2, phy1);
        mem->write(e, phy2);
      }
    }
    return ok();
  };
  if (mem->count<CollisionArray>()) {
    return mem->process<CollisionArray>(work);
  }
  return ok();
}

}  // namespace arty
