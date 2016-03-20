#pragma once

#include <stdint.h>

#include "Simulatable.h"
#include "ActionTimeAccumulator.h"

enum ControllableControls
{
  CONTROLS_WASD,
  CONTROLS_ARROWS
};

class ControllableCharacter : public Simulatable
{
public:
  ControllableCharacter();
  ControllableCharacter(sf::Vector2f start, sf::Vector2f size, ControllableControls controls, uint32_t id);
  ~ControllableCharacter();

  virtual void PrepareUpdate(REAL delta);
  virtual void PreUpdate(REAL delta);

  void UpdateAccumulators(REAL delta);
  sf::CircleShape& GetDrawable();

  void ExecuteActionDown();
  void ExecuteActionUp();
  void ExecuteActionRight();
  void ExecuteActionLeft();

private:
  void ApplyInputs();
  void LoadSettings();

  sf::RectangleShape mRect;

  ActionTimeAccumulator mUpActionAccumulator;
  ActionTimeAccumulator mDownActionAccumulator;
  ActionTimeAccumulator mRightActionAccumulator;
  ActionTimeAccumulator mLeftActionAccumulator;
  ActionTimeAccumulator mSpaceActionAccumulator;
  ActionTimeAccumulator mParticleSpawnAccumulator;

  ControllableControls mControls;
  sf::CircleShape mDrawable;

  unsigned int mRewind;
};

