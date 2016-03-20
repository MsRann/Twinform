#pragma once
#include "Simulatable.h"

#include "PlayerControls.h"
#include "Brain.h"

class Neutron : public Simulatable
{
public:
  Neutron(Brain* brain, sf::Vector2f start, sf::Vector2f size, uint32_t id);
  virtual ~Neutron() override;

  virtual void PrepareUpdate(REAL delta) override;
  virtual void PreUpdate(REAL delta) override;

  sf::CircleShape& GetDrawable();

private:
  sf::CircleShape mDrawable;
  Brain* mBrain;
};

