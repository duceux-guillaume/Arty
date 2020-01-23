#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Memory>& mem) {
  auto work = [mem, this](Entity const& e, Physics const& p) -> Result {
    if (p.isStatic()) {
      mem->write(e, OUTPUT, p.position());
      return ok();
    }
    auto newPhy = p;
    _solver.update(_world, &newPhy);
    mem->write(e, OUTPUT, newPhy.position());
    mem->write(e, INPUT, newPhy);
    return ok();
  };
  mem->process<Physics>(INPUT, work);
  return ok();
}

}  // namespace arty
