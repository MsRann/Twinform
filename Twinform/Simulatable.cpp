#include "Simulatable.h"

#include "Renderer.h"
#include "Simulator.h"

Simulatable::Simulatable() :
  mParticle(sf::Vector2f(0.0f, 0.0f))
  , mCollisionBounds(sf::FloatRect(0.0f, 0.0f, 25.0f, 25.0f))
  , mGravity(sf::Vector2f(0.0f, 0.0f))
  , mPast(10) // Will store the last 10 physics steps worth of particles
  , mID(0)
  , mFlags(0)
{
}

Simulatable::Simulatable(sf::Vector2f start, sf::Vector2f size) :
  mParticle(start)
  , mCollisionBounds(start.x, start.y, size.x, size.y)
  , mGravity(sf::Vector2f(0.0f, 0.0f))
  , mPast(10) // Will store the last 10 physics steps worth of particles
  , mID(0)
  , mFlags(0)
{
}

Simulatable::~Simulatable()
{
}

void Simulatable::PostUpdate(REAL delta)
{
  mPast.Add(mParticle);
  if (mFlags & HAS_GRAVITY)
    mParticle.AddAcceleration(mGravity);
  mParticle.Integrate(delta);
}

void Simulatable::UpdateCollisionBounds()
{
  sf::Vector2f pos = mParticle.GetPosition();
  mCollisionBounds.left = pos.x;
  mCollisionBounds.top = pos.y;
}

void Simulatable::SetGravity(const sf::Vector2f& gravity)
{
  mFlags |= HAS_GRAVITY;
  mGravity = gravity;
}

void Simulatable::SetPrevKey(const sf::Vector2i& key)
{
  mPrevKey = key;
}

const sf::FloatRect& Simulatable::GetCollisionBounds() const
{
  return mCollisionBounds;
}

sf::FloatRect Simulatable::EstimateCollisionBounds(const sf::Vector2f& position) const
{
  sf::FloatRect bounds = mCollisionBounds;
  bounds.top = position.y;
  bounds.left = position.x;
  return bounds;
}

sf::Vector2i Simulatable::GetPrevKey() const
{
  return mPrevKey;
}

void Simulatable::SetParticle(const Particle& particle)
{
  mParticle = particle;
}

const sf::Vector2f& Simulatable::GetGravity() const
{
  return mGravity;
}

void Simulatable::SetFlag(PhysicalFlags flag)
{
  mFlags |= flag;
}

void Simulatable::UnsetFlag(PhysicalFlags flag)
{
  mFlags &= ~flag;
}

Particle& Simulatable::GetParticle()
{
  return mParticle;
}

Particle Simulatable::GetParticleCopy()
{
  return mParticle;
}

const Particle& Simulatable::GetParticleConst() const
{
  return mParticle;
}

unsigned int Simulatable::GetFlags() const
{
  return mFlags;
}

REAL Simulatable::GetMass() const
{
  return mParticle.GetMass();
}  

RingBuffer<Particle>& Simulatable::GetPast()
{
  return mPast;
}

uint32_t Simulatable::GetID()
{
  return mID;
}