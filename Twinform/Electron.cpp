#include "Electron.h"

#include "Settings.h"

#include <cassert>

Electron::Electron(
  Brain* brain
  , sf::Vector2f start
  , sf::Vector2f size
  , uint32_t id) :
  Simulatable(start, size)
  , mBrain(brain)
{
  // Neutrons need brains to control themselves
  assert(mBrain);
  mID = id;
  mDrawable.setPosition(start);
  mDrawable.setRadius(Settings::GetCharacterSettings().mRadius);
  mBrain->Initialize(this);
}

Electron::~Electron()
{
}

void Electron::PreUpdate(REAL delta)
{
  assert(mBrain);
  mBrain->Update(delta);
  sf::Vector2f pos = mParticle.GetPosition();
  mDrawable.setPosition(pos);
  UpdateCollisionBounds();
}

sf::CircleShape& Electron::GetDrawable()
{
  return mDrawable;
}

