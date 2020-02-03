#include <arty/impl/camera_system.hpp>
#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Memory>& mem) {
  auto r = resolveCollision(mem);
  if (!r) {
    return r;
  }
  return integrateMotion(mem);
}

Result PhysicsSystem::integrateMotion(const Ptr<Memory>& mem) const {
  auto work = [mem, this](Entity const& e, Particle const& p) -> Result {
    Particle np = p;
    _integrator->integrate(np, 0.01666666666);
    if (np.position.z() < -5) {
      mem->remove(e);
    } else {
      mem->write(e, np);
      mem->write(e, Tf3f(static_cast<Vec3f>(np.position)));
    }
    return ok();
  };
  return mem->process<Particle>(work);
}

Result PhysicsSystem::resolveCollision(const Ptr<Memory>& mem) const {
  auto work = [mem](Entity const& e, CollisionArray const& buffer) -> Result {
    CollisionSolver solver;
    for (auto const& c : buffer) {
      Particle p1, p2;
      if (!mem->read(c.entities().first, p1)) {
        return ok();
      }
      if (!mem->read(c.entities().second, p2)) {
        return ok();
      }
      if (e == c.entities().second) {
        std::swap(p1, p2);
      }
      solver.resolve(c, p1, p2);
      mem->write(c.entities().first, p1);
      mem->write(c.entities().second, p2);
    }
    return ok();
  };
  if (mem->count<CollisionArray>()) {
    return mem->process<CollisionArray>(work);
  }
  return ok();
}

Result CollisionDetectionSystem::process(Ptr<Memory> const& mem) {
  mem->remove<CollisionArray>();
  auto first_loop = [mem, this](Entity const& e1, Tf3f const& t,
                                AABox3f const& b) -> Result {
    auto second_loop = [mem, this, e1, t, b](Entity const& e2, Tf3f const& t2,
                                             AABox3f const& b2) -> Result {
      if (e1 >= e2) {
        return ok();
      }

      Collision col = _collision.detect(t, b, t2, b2);
      if (col.exist()) {
        col.set(e1, e2);
        CollisionArray cols1, cols2;
        // no previous collision is fine
        (void)mem->read<CollisionArray>(e1, cols1);
        cols1.push_back(col);
        mem->write(e1, cols1);
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

  auto work = [&](Entity const& e, CollisionArray const& b) -> Result {
    for (auto const& col : b) {
      std::vector<Vec3f> lines;
      auto c = static_cast<Vec3f>(col.center());
      auto n = static_cast<Vec3f>(col.normal() * col.penetration());
      lines.push_back(c);
      lines.push_back(c + n);
      _renderer->draw(e, lines, Mat4x4f::identity(), cam.view(),
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

}  // namespace arty
