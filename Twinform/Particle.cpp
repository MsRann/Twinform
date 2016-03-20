#include <assert.h>
#include <math.h>
#include <iostream>
#include "Particle.h"


Particle::Particle() :
  mPosition(0.0f, 0.0f)
  , mVelocity(0.0f, 0.0f)
  , mAcceleration(0.0f, 0.0f)
  , mForceAccumulator(0.0f, 0.0f)
  , mInverseMass(100.0f)
  , mDamping(0.1f)
{
}

Particle::Particle(const sf::Vector2f position) :
  mPosition(position)
  , mVelocity(0.0f, 0.0f)
  , mAcceleration(0.0f, 0.0f)
  , mForceAccumulator(0.0f, 0.0f)
  , mInverseMass(100.0f)
  , mDamping(0.1f)
{
}

Particle::~Particle()
{
}

// Is euler integration that great here?
// Small enough deltas should result in OK results.
// TODO: Lets research this more deeply someday
void Particle::Integrate(REAL duration)
{
  // Don't integrate things with 0 mass
  if (mInverseMass <= FLOAT_SMALL) return;

  // Don't integrate with very very small values
  if (duration < FLOAT_SMALL) return;

  // Update linear position
  mPosition += mVelocity * duration;

  // Compute acceleration for force F = ma
  // a = F / m = F * (1/m)
  sf::Vector2f resultAcceleration = mAcceleration;
  resultAcceleration += mForceAccumulator * mInverseMass;

  // Update linear velocity from acceleration
  mVelocity += resultAcceleration * duration;

  // Impose drag
  mVelocity *= pow(mDamping, duration);

  // Clear the forces
  ClearAccumulator();
}

void Particle::Translate(const sf::Vector2f& offset)
{
  mPosition += offset;
}

void Particle::SetMass(const REAL mass)
{
  assert(mass > FLOAT_SMALL);
  mInverseMass = ((REAL)1.0f) / mass;
}

REAL Particle::GetMass() const
{
  if (mInverseMass < FLOAT_SMALL)
    return MASS_MAX;

  return ((REAL)1.0f) / mInverseMass;
}

void Particle::SetInverseMass(const REAL inverseMass)
{
  mInverseMass = inverseMass;
}

REAL Particle::GetInverseMass() const
{
  return mInverseMass;
}

bool Particle::HasFiniteMass() const
{
  return mInverseMass >= INVERSE_MASS_SMALL;
}

void Particle::SetDamping(const REAL damping)
{
  mDamping = damping;
}

REAL Particle::GetDamping() const
{
  return mDamping;
}

void Particle::SetPosition(const sf::Vector2f& position)
{
  mPosition = position;
}

void Particle::SetPosition(const REAL x, const REAL y)
{
  mPosition.x = x;
  mPosition.y = y;
}

sf::Vector2f Particle::GetPosition() const
{
  return mPosition;
}

void Particle::SetVelocity(const sf::Vector2f& velocity)
{
  mVelocity = velocity;
}

void Particle::SetVelocity(const REAL x, const REAL y)
{
  mVelocity.x = x;
  mVelocity.y = y;
}

sf::Vector2f Particle::GetVelocity() const
{
  return mVelocity;
}

void Particle::AddVelocity(const sf::Vector2f& velocity)
{
  mVelocity += velocity;
}

void Particle::AddVelocity(const REAL x, const REAL y)
{
  mVelocity += sf::Vector2f(x, y);
}

void Particle::DampenVelocity(const REAL scalar)
{
  mVelocity.x *= scalar;
  mVelocity.y *= scalar;
}

void Particle::SetAcceleration(const sf::Vector2f& acceleration)
{
  mAcceleration = acceleration;
}

void Particle::SetAcceleration(const REAL x, const REAL y)
{
  mAcceleration.x = x;
  mAcceleration.y = y;
}

void Particle::AddAcceleration(const sf::Vector2f& acceleration)
{
  mAcceleration += acceleration;
}

void Particle::AddAcceleration(const REAL x, const REAL y)
{
  mAcceleration += sf::Vector2f(x, y);
}

sf::Vector2f Particle::GetAcceleration() const
{
  return mAcceleration;
}

void Particle::DampenAcceleration(const REAL scalar)
{
  mAcceleration.x *= scalar;
  mAcceleration.y *= scalar;
}

void Particle::ClearAccumulator()
{
  mForceAccumulator.x = 0.0f;
  mForceAccumulator.y = 0.0f;
}

void Particle::AddForce(const sf::Vector2f& force)
{
  mForceAccumulator += force;
}

void Particle::ZeroMovement()
{
  mAcceleration = sf::Vector2f(0.0f, 0.0f);
  mVelocity = sf::Vector2f(0.0f, 0.0f  );
  ClearAccumulator();
}
