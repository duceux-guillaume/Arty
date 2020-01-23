#include <arty/impl/camera_system.hpp>
#include <arty/impl/collision_system.hpp>

namespace arty {

Result CollisionDetectionSystem::process(Ptr<Memory> const& mem) {
  mem->remove(OUTPUT);
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
        std::cout << "found collision between " << e.name() << " and "
                  << e2.name() << std::endl;
        auto cols1 = mem->read<std::vector<Collision>>(e, OUTPUT);
        cols1.push_back(col);
        mem->write(e, OUTPUT, cols1);

        auto cols2 = mem->read<std::vector<Collision>>(e2, OUTPUT);
        cols2.push_back(col);
        mem->write(e2, OUTPUT, cols2);
      }
      return ok();
    };
    return mem->process<Tf3f, AABox3f>(INPUT_1, INPUT_2, second_loop);
  };
  return mem->process<Tf3f, AABox3f>(INPUT_1, INPUT_2, first_loop);
}

Result CollisionRenderingSystem::process(const Ptr<Memory>& mem) {
  auto cam = mem->read<Camera>("camera");

  auto work = [&](Entity const& e, std::vector<Collision> const& b) -> Result {
    for (auto const& col : b) {
      _renderer->draw(e, col.intersection(), Mat4x4f::identity(), cam.view,
                      cam.projection);
    }
    return ok();
  };
  if (mem->count(INPUT) > 0) {
    return mem->process<std::vector<Collision>>(INPUT, work);
  }
  return ok();
}

Result CollisionRenderingSystem::init(const Ptr<Memory>&) {
  return _renderer->init();
}

Result CollisionSolverSystem::process(Ptr<Memory> const& mem) {
  auto work = [&](Entity const& e, std::vector<Collision> const& b) -> Result {
    for (auto const& col : b) {
      auto phy1 =
          mem->read<Physics>(col.entities().first, PhysicsSystem::INPUT);
      auto phy2 =
          mem->read<Physics>(col.entities().second, PhysicsSystem::INPUT);
      if (e == col.entities().first) {
        _solver.solve(col, &phy1, phy2);
        mem->write(e, PhysicsSystem::INPUT, phy1);
      } else {
        _solver.solve(col, &phy2, phy1);
        mem->write(e, PhysicsSystem::INPUT, phy2);
      }
    }
    return ok();
  };
  if (mem->count(INPUT) > 0) {
    return mem->process<std::vector<Collision>>(INPUT, work);
  }
  return ok();
}

Result CollisionSolverSystem::init(const Ptr<Memory>& /*board*/) {
  return ok();
}

void CollisionSolverSystem::release() {}

}  // namespace arty
