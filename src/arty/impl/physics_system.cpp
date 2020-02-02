#include <arty/impl/physics_system.hpp>

namespace arty {

Result PhysicsSystem::process(const Ptr<Memory>& mem) {
  return integrateMotion(mem);
}

Result PhysicsSystem::integrateMotion(const Ptr<Memory>& mem) {
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

}  // namespace arty
