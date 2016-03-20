#pragma once

#include <stdint.h>

#include "Simulatable.h"
#include "ActionTimeAccumulator.h"
#include "PlayerControls.h"

class ControllableCharacter : public Simulatable
{
public:
  ControllableCharacter();
  ControllableCharacter(sf::Vector2f start, sf::Vector2f size, ControllableControls controls, uint32_t id);
  ~ControllableCharacter();

  virtual void PrepareUpdate(REAL delta);
  virtual void PreUpdate(REAL delta);

  sf::CircleShape& GetDrawable();

private:
  sf::RectangleShape mRect;
  PlayerControls mControls;

  sf::CircleShape mDrawable;

  unsigned int mRewind;
};

