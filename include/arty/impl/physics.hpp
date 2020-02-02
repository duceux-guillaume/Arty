#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <arty/core/geometry.hpp>
#include <arty/core/number.hpp>
#include <vector>

namespace arty {

struct Particle {
  using number_t = number;
  using vector_t = Vec3<number_t>;

  vector_t position;
  vector_t velocity;
  number_t damping;
  number_t invmass;
  vector_t gravity;
  vector_t forceaccu;
};

struct Integrator {
  virtual void integrate(Particle& p, double time) const = 0;
};
class FastIntegrator : public Integrator {
 public:
  void integrate(Particle& p, double time) const override;
};
class AccurateIntegrator : public Integrator {
 public:
  void integrate(Particle& p, double time) const override;
};

}  // namespace arty

#endif  // PHYSICS_HPP
