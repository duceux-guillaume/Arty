#include <arty/impl/physics.hpp>

namespace arty {

void FastIntegrator::integrate(Particle& p, double duration) const {
  assert(duration > 0.);
  Particle::number_t dt(duration);
  auto acceleration = p.gravity + p.forceaccu * p.invmass;
  p.velocity = p.velocity * p.damping + acceleration * dt;
  p.position += p.velocity * dt;
  p.forceaccu = Particle::vector_t();
}

void AccurateIntegrator::integrate(Particle& p, double duration) const {
  assert(duration > 0.);
  Particle::number_t dt(duration);
  Particle::number_t half(0.5);
  auto acceleration = p.gravity + p.forceaccu * p.invmass;
  auto powd = Particle::number_t::pow(p.damping, dt, 1);
  p.velocity = p.velocity * powd + acceleration * dt;
  p.position += p.velocity * dt + acceleration * dt * dt * half;
  p.forceaccu = Particle::vector_t();
}

}  // namespace arty
