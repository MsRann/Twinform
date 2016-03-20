#pragma once
#include "Brain.h"

#include "TwinformTypes.h"
#include "ActionTimeAccumulator.h"

enum ControllableControls
{
  CONTROLS_WASD,
  CONTROLS_ARROWS
};

class PlayerControls :
  public Brain
{
public:
  PlayerControls(ControllableControls controls);
  ~PlayerControls();

  // Inherited via Brain
  virtual void Update(REAL delta) override;

  void ExecuteActionDown();
  void ExecuteActionUp();
  void ExecuteActionRight();
  void ExecuteActionLeft();

private:
  void UpdateAccumulators(REAL delta);
  void ApplyInputs();

  ActionTimeAccumulator mUpActionAccumulator;
  ActionTimeAccumulator mDownActionAccumulator;
  ActionTimeAccumulator mRightActionAccumulator;
  ActionTimeAccumulator mLeftActionAccumulator;
  ActionTimeAccumulator mSpaceActionAccumulator;
  ControllableControls mControls;
};

