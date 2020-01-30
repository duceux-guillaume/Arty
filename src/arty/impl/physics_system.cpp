#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Memory>& mem) {
  auto work = [mem, this](Entity const& e, Physics const& p) -> Result {
    if (p.isStatic()) {
      mem->write(e, p.position());
      return ok();
    }
    auto newPhy = p;
    _solver.update(_world, &newPhy);
    mem->write(e, newPhy.position());
    mem->write(e, newPhy);
    return ok();
  };
  mem->process<Physics>(work);
  return ok();
}

}  // namespace arty
