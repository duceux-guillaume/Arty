#include <arty/impl/physics.hpp>

namespace arty {

void FastIntegrator::integrate(Particle& p, double duration) const {
  assert(duration > 0.);
  if (p.isStatic()) {
    return;
  }
  number_t dt(duration);
  auto acceleration = p.gravity + p.forceaccu * p.inverseMass();
  p.velocity = p.velocity * p.damping + acceleration * dt;
  p.position += p.velocity * dt;
  p.forceaccu = vector_t();
}

void AccurateIntegrator::integrate(Particle& p, double duration) const {
  assert(duration > 0.);
  if (p.isStatic()) {
    return;
  }
  number_t dt(duration);
  number_t half(0.5);
  auto acceleration = p.gravity + p.forceaccu * p.inverseMass();
  using std::pow;
  auto powd = pow(p.damping, dt);
  p.velocity = p.velocity * powd + acceleration * dt;
  p.position += p.velocity * dt + acceleration * dt * dt * half;
  p.forceaccu = vector_t();
}

Collision CollisionDetection::detect(const Tf3f& tf1, const AABox3f& b1,
                                     const Tf3f& tf2, const AABox3f& b2) {
  AABox3f rb1 = b1.move(tf1);
  AABox3f rb2 = b2.move(tf2);
  auto intersection = rb1.intersection(rb2);
  if (intersection.empty()) {
    return Collision();
  }
  // Compute sign and stuff
  vector_t center = static_cast<vector_t>(intersection.value().center());
  auto const& v = intersection.value().halfLength();
  vector_t normal = static_cast<vector_t>(
      (tf1.translation() - tf2.translation()).normalize());
  number_t penetration;
  if (v.x() <= v.y() && v.x() <= v.z()) {
    penetration = v.x();
    normal = vector_t{1.f, 0.f, 0.f};
    if (tf1.translation().x() < tf2.translation().x()) {
      normal *= number_t(-1);
    }
  }
  if (v.y() <= v.x() && v.y() <= v.z()) {
    penetration = v.y();
    normal = vector_t{0.f, 1.f, 0.f};
    if (tf1.translation().y() < tf2.translation().y()) {
      normal *= number_t(-1);
    }
  }
  penetration = v.z();
  normal = vector_t{0.f, 0.f, 1.f};
  if (tf1.translation().z() < tf2.translation().z()) {
    normal *= number_t(-1);
  }
  return Collision(normal, center, penetration);
}

void CollisionSolver::resolveVelocity(Collision const& c, Particle& p1,
                                      Particle& p2) const {
  if (p1.isStatic() && p2.isStatic()) {
    return;
  }
  // Find the velocity in the direction of the contact.
  vector_t relativeVelocity = p1.velocity - p2.velocity;
  vector_t normal = c.normal();
  number_t separatingVelocity = relativeVelocity.dot(normal);
  // Check whether it needs to be resolved.
  if (separatingVelocity > 0) {
    // The contact is either separating or stationary - there’s
    // no impulse required.
    return;
  }
  // Calculate the new separating velocity.
  number_t newSepVelocity =
      -separatingVelocity * (p1.restitution + p2.restitution);
  number_t deltaVelocity = newSepVelocity - separatingVelocity;
  // We apply the change in velocity to each object in proportion to
  // its inverse mass (i.e., those with lower inverse mass [higher
  // actual mass] get less change in velocity).
  number_t totalInverseMass = 0;
  if (!p1.isStatic()) {
    totalInverseMass += p1.inverseMass();
  }
  if (!p2.isStatic()) {
    totalInverseMass += p2.inverseMass();
  };
  // If all particles have infinite mass, then impulses have no effect.
  if (totalInverseMass <= 0) return;
  // Calculate the impulse to apply.
  number_t impulse = deltaVelocity / totalInverseMass;
  // Find the amount of impulse per unit of inverse mass.
  vector_t impulsePerIMass = c.normal() * impulse;
  // Apply impulses: they are applied in the direction of the contact,
  // and are proportional to the inverse mass.
  if (!p1.isStatic()) {
    p1.velocity = p1.velocity + impulsePerIMass * p1.inverseMass();
  }
  if (!p2.isStatic()) {
    // Particle 2 goes in the opposite direction.
    p2.velocity = p2.velocity - impulsePerIMass * p2.inverseMass();
  }
}

void CollisionSolver::resolvePenetration(Collision const& c, Particle& p1,
                                         Particle& p2) const {
  // If we don’t have any penetration, skip this step.
  // if (c.penetration <= 0) return;
  // If all particles have infinite mass, then we do nothing.
  if (p1.isStatic() && p2.isStatic()) {
    return;
  }
  // The movement of each object is based on its inverse mass, so
  // total that.
  number_t totalInverseMass = 0;
  if (!p1.isStatic()) {
    totalInverseMass += p1.inverseMass();
  }
  if (!p2.isStatic()) {
    totalInverseMass += p2.inverseMass();
  }
  // Find the amount of penetration resolution per unit of inverse mass.
  vector_t movePerIMass = c.normal() * (-c.penetration() / totalInverseMass);
  // Apply the penetration resolution.
  if (!p1.isStatic()) {
    p1.position += movePerIMass * p1.inverseMass();
  }
  if (!p2.isStatic()) {
    p2.position += movePerIMass * p2.inverseMass();
  }
}

void CollisionSolver::resolve(Collision const& c, Particle& p1,
                              Particle& p2) const {
  resolveVelocity(c, p1, p2);
  resolvePenetration(c, p1, p2);
}

}  // namespace arty
