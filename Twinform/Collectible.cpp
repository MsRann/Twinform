#include "Collectible.h"

Collectible::Collectible() :
  Simulatable()
{
  SetFlag(CAN_PICKUP);
}

Collectible::Collectible(sf::Vector2f start, sf::Vector2f size, uint32_t id) :
  Simulatable(start, size)
{
  // TODO: Move id somewhere else
  mID = id;
  mDrawable.setPosition(start);
  mDrawable.setSize(size);
  SetFlag(CAN_PICKUP);
}


Collectible::~Collectible()
{
}

void Collectible::PreUpdate(REAL delta)
{
  mDrawable.rotate(5.0f);
}

void Collectible::PostUpdate(REAL delta)
{
  
}

sf::RectangleShape& Collectible::GetDrawable()
{
  return mDrawable;
}