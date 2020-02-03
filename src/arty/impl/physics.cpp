#include <arty/impl/physics.hpp>

namespace arty {

Collision Physics::detectCollision(const Tf3f& tf1, const AABox3f& b1,
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
  vector_t normal;
  number_t penetration;
  if (v.x() <= v.y() && v.x() <= v.z()) {
    penetration = v.x() * 2;
    normal.x() = number_t(1);
    if (tf1.translation().x() < tf2.translation().x()) {
      normal.x() = number_t(-1);
    }
  } else if (v.y() <= v.x() && v.y() <= v.z()) {
    penetration = v.y() * 2;
    normal.y() = number_t(1);
    if (tf1.translation().y() < tf2.translation().y()) {
      normal.y() = number_t(-1);
    }
  } else {
    penetration = v.z() * 2;
    normal.z() = number_t(1);
    if (tf1.translation().z() < tf2.translation().z()) {
      normal.z() = number_t(-1);
    }
  }
  return Collision(normal, center, penetration);
}

void Physics::resolveVelocity(Collision const& c, Particle& p1, Particle& p2,
                              double duration) const {
  if (p1.isStatic() && p2.isStatic()) {
    return;
  }

  // Find the velocity in the direction of the contact.
  number_t sepVel = separatingVelocity(c, p1, p2);

  // Check whether it needs to be resolved.
  if (sepVel > 0) {
    // The contact is either separating or stationary - there’s
    // no impulse required.
    return;
  }

  // Calculate the new separating velocity.
  number_t restitution = p1.restitution + p2.restitution;
  number_t newSepVelocity = -sepVel * restitution;
  // Check the velocity build-up due to acceleration only.
  vector_t accCausedVelocity = p1.gravity - p2.gravity;
  number_t accCausedSepVelocity = accCausedVelocity.dot(c.normal()) * duration;

  // If we’ve got a closing velocity due to acceleration build-up,
  // remove it from the new separating velocity.
  if (accCausedSepVelocity < 0) {
    newSepVelocity += restitution * accCausedSepVelocity;
    // Make sure we haven’t removed more than was
    // there to remove.
    if (newSepVelocity < 0) {
      newSepVelocity = 0;
    }
  }

  // We apply the change in velocity to each object in proportion to
  // its inverse mass (i.e., those with lower inverse mass [higher
  // actual mass] get less change in velocity).
  number_t deltaVelocity = newSepVelocity - sepVel;
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

void Physics::resolvePenetration(Collision const& c, Particle& p1,
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

void Physics::resolve(Collision const& c, Particle& p1, Particle& p2,
                      double duration) const {
  resolveVelocity(c, p1, p2, duration);
  // resolvePenetration(c, p1, p2);
}

number_t Physics::separatingVelocity(Collision const& c, const Particle& p1,
                                     const Particle& p2) const {
  vector_t relativeVelocity = p1.velocity - p2.velocity;
  return relativeVelocity.dot(c.normal());
}

void Physics::integrateMotion(Particle& p, double duration) const {
  assert(duration > 0.);
  if (p.isStatic()) {
    return;
  }
  number_t dt(duration);
  p.position += p.velocity * dt;
  auto acceleration = p.gravity + p.forceaccu * p.inverseMass();
  using std::pow;
  auto powd = pow(p.damping, dt);
  p.velocity = p.velocity * powd + acceleration * dt;
  p.forceaccu = vector_t();
}

}  // namespace arty
