#include "Neutron.h"

Neutron::Neutron(Brain* brain) :
  mBrain(brain)
{
}

Neutron::~Neutron()
{
  delete mBrain;
}

void Neutron::PrepareUpdate(REAL delta)
{
}

void Neutron::PreUpdate(REAL delta)
{
}
