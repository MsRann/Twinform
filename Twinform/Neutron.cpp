#include "Neutron.h"

#include <cassert>

#include "Settings.h"
#include "Commands.h"
#include "CommandStream.h"

Neutron::Neutron(
  Brain* brain
  , sf::Vector2f start
  , sf::Vector2f size
  , uint32_t id) :
  Simulatable(start, size)
  , mBrain(brain)
  , mSplitActionAccumulator(0.5f)
{
  // Neutrons need brains to control themselves
  assert(mBrain);
  mID = id;
  mDrawable.setPosition(start);
  mDrawable.setRadius(Settings::GetCharacterSettings().mRadius);
  mBrain->Initialize(this);
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
  assert(mBrain);
  ExecuteControls();
  mBrain->Update(delta);
  sf::Vector2f pos = mParticle.GetPosition();
  mDrawable.setPosition(pos);
  UpdateCollisionBounds();
}

sf::CircleShape& Neutron::GetDrawable()
{
  return mDrawable;
}

void Neutron::ExecuteControls()
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
  {
    if (!mSplitActionAccumulator.IsReady()) return;
    // Command will split the neutron and delete it
    CommandStream::Add(new SplitNeutronCommand(this));
    mSplitActionAccumulator.Reset();
  }
}
