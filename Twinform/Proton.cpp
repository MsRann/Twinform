#include "Proton.h"

#include "Settings.h"

#include <cassert>

Proton::Proton(
  Brain * brain
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
  mDrawable.setFillColor(sf::Color::Blue);
  mDrawable.setOutlineThickness(1.0f);
  mDrawable.setOutlineColor(sf::Color::Black);
  mBrain->Initialize(this);
}

Proton::~Proton()
{
}

void Proton::PreUpdate(REAL delta)
{
  assert(mBrain);
  mBrain->Update(delta);
  sf::Vector2f pos = mParticle.GetPosition();
  mDrawable.setPosition(pos);
  UpdateCollisionBounds();
}

sf::CircleShape& Proton::GetDrawable()
{
  return mDrawable;
}
