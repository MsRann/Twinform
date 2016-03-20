#include "ControllableCharacter.h"
#include "Renderer.h"
#include "Simulator.h"
#include "PropertyReader.h"
#include "ParticleSystem.h"
#include "TwinMath.h"
#include "Settings.h"

#include <SFML\Window\Keyboard.hpp>

#include <iostream> // TODO: Remove me


ControllableCharacter::ControllableCharacter() :
  Simulatable()
  , mControls(CONTROLS_WASD)
{
  mControls.Initialize(this);
}

ControllableCharacter::ControllableCharacter(sf::Vector2f start
  , sf::Vector2f size
  , ControllableControls controls
  , uint32_t id) :
  Simulatable(start, size)
  , mControls(controls)
  , mRewind(0)
{
  // TODO: Move id somewhere else
  mID = id;
  mDrawable.setPosition(start);
  mDrawable.setRadius(Settings::GetCharacterSettings().mRadius);
  mControls.Initialize(this);
}

ControllableCharacter::~ControllableCharacter()
{
}

void ControllableCharacter::PrepareUpdate(REAL delta)
{
  mControls.Update(delta);
}

void ControllableCharacter::PreUpdate(REAL delta)
{
  sf::Vector2f pos = mParticle.GetPosition();
  mDrawable.setPosition(pos);
  UpdateCollisionBounds();
}

sf::CircleShape& ControllableCharacter::GetDrawable()
{
  return mDrawable;
}