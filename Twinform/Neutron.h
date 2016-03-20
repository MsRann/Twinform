#pragma once
#include "Simulatable.h"

#include "Brain.h"

class Neutron : public Simulatable
{
public:
  Neutron(Brain* brain);
  virtual ~Neutron() override;

  virtual void PrepareUpdate(REAL delta) override;
  virtual void PreUpdate(REAL delta) override;

private:
  Brain* mBrain;
};

