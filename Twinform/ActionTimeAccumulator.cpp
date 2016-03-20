#include "ActionTimeAccumulator.h"

ActionTimeAccumulator::ActionTimeAccumulator() :
mLimit(0.0f)
, mAccumulation(0.0f)
{

}

ActionTimeAccumulator::ActionTimeAccumulator(REAL limit) :
mLimit(limit)
, mAccumulation(limit) // start off at limit so first action is allowed
{

}

void ActionTimeAccumulator::Add(REAL delta)
{
  mAccumulation += delta;
}

void ActionTimeAccumulator::Subtract(REAL delta)
{
  mAccumulation -= delta;
}

void ActionTimeAccumulator::SetLimit(REAL limit)
{
  mLimit = limit;
}

void ActionTimeAccumulator::Reset()
{
  mAccumulation = 0.0f;
}

bool ActionTimeAccumulator::IsReady()
{
  if (mAccumulation >= mLimit - FLOAT_SMALL)
    return true;

  return false;
}

REAL ActionTimeAccumulator::GetAccumulation()
{
  return mAccumulation;
}