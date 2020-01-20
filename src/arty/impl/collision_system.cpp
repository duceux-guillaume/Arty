#include <arty/core/mesh.hpp>
#include <arty/impl/collision_system.hpp>

namespace arty {
Result CollisionSystem::process(Ptr<Memory> const& mem) {
  mem->remove(OUTPUT);
  auto first_loop = [mem, this](Entity const& e, Transform const& t,
                                Box const& b) -> Result {
    auto second_loop = [mem, this, e, t, b](Entity const& e2,
                                            Transform const& t2,
                                            Box const& b2) -> Result {
      if (e >= e2) {
        return ok();
      }

      Collision col = _collision.detect(t, b, t2, b2);
      if (col.exist) {
        std::cout << "found collision between " << e.name() << " and "
                  << e2.name() << std::endl;
        mem->write(e, OUTPUT, col);
        mem->write(e2, OUTPUT, col);
      }
      return ok();
    };
    return mem->process<Transform, Box>(INPUT_1, INPUT_2, second_loop);
  };
  return mem->process<Transform, Box>(INPUT_1, INPUT_2, first_loop);
}

}  // namespace arty
