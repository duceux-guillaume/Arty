#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <arty/core/system.hpp>
#include <arty/impl/physics.hpp>

namespace arty {

class PhysicsSystem : public System {
 public:
  PhysicsSystem(Ptr<Integrator> integrator) : _integrator(integrator) {}

 private:
  Ptr<Integrator> _integrator;
  // System interface
 public:
  Result process(const Ptr<Memory>& board) override;

 public:
  Result integrateMotion(Ptr<Memory> const& mem);
};

}  // namespace arty

#endif  // PHYSICS_SYSTEM_HPP
