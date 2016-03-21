#pragma once

#include "Simulatable.h"

#include "Brain.h"

class Proton :
  public Simulatable
{
public:
  Proton(
    Brain* brain
    , sf::Vector2f start
    , sf::Vector2f size
    , uint32_t id);
  virtual ~Proton() override;

  // Inherited via Simulatable
  virtual void PreUpdate(REAL delta) override;

  sf::CircleShape& GetDrawable();

private:
  sf::CircleShape mDrawable;

  REAL mCharge;
  Brain* mBrain;
};

