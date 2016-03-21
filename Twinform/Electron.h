#pragma once

#include "Simulatable.h"

#include "Brain.h"

class Electron :
  public Simulatable
{
public:
  Electron(
    Brain* brain
    , sf::Vector2f start
    , sf::Vector2f size
    , uint32_t id);
  virtual ~Electron() override;

  // Inherited via Simulatable
  virtual void PreUpdate(REAL delta) override;

  sf::CircleShape& GetDrawable();

private:
  sf::CircleShape mDrawable;

  REAL mCharge;
  Brain* mBrain;
};

