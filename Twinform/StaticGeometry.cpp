#include "StaticGeometry.h"
#include "Renderer.h"
#include "Simulator.h"

StaticGeometry::StaticGeometry() :
  Simulatable(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f))
{
}

StaticGeometry::StaticGeometry(sf::Vector2f startPosition, sf::Vector2f size, uint32_t id) :
  Simulatable(startPosition, size)
{
  mID = id;
  mFlags |= STATIC_GEOMETRY;

  mDrawable.setPosition(startPosition);
  mDrawable.setSize(size);
}


StaticGeometry::~StaticGeometry()
{
}


void StaticGeometry::PreUpdate(REAL delta)
{
  mDrawable.setPosition(mParticle.GetPosition());
}

sf::RectangleShape& StaticGeometry::GetDrawable()
{
  return mDrawable;
}