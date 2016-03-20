#pragma once

#include "Simulatable.h"

// Brain will be the base class for all characters
// requiring either AI control or player control.
class Brain
{
public:
  virtual void Initialize(Simulatable* sim);
  virtual void Update(REAL delta) = 0;

protected:
  Simulatable* mSimulatable;
};

