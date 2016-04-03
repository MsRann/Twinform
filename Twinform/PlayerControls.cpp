#include "PlayerControls.h"

#include "PropertyReader.h"
#include "Settings.h"

namespace
{
  static Settings::CharacterSettings sCharacterSettings;
}

PlayerControls::PlayerControls(ControllableControls controls) :
  mControls(controls)
  , mUpActionAccumulator(0.50f)
  , mRightActionAccumulator(0.05f)
  , mLeftActionAccumulator(0.05f)
  , mDownActionAccumulator(0.50f)
  , mSpaceActionAccumulator(MAX_DT)
{
  sCharacterSettings = Settings::GetCharacterSettings();
}


PlayerControls::~PlayerControls()
{
}

void PlayerControls::Update(REAL delta)
{
  UpdateAccumulators(delta);
  ApplyInputs();
}

void PlayerControls::ExecuteActionDown()
{
  if (!mDownActionAccumulator.IsReady())
    return;
}

void PlayerControls::ExecuteActionUp()
{
  if (!mUpActionAccumulator.IsReady())
    return;

  float jumpForce = sCharacterSettings.mJumpForceY;
  if (mSimulatable->mGravity.y < 0.0f)
    jumpForce *= -1;

  mSimulatable->GetParticle().AddForce(
    sf::Vector2f(sCharacterSettings.mJumpForceX
      , jumpForce));

  mUpActionAccumulator.Reset();
}

void PlayerControls::ExecuteActionRight()
{
  if (!mRightActionAccumulator.IsReady())
    return;

  mSimulatable->GetParticle().AddVelocity(
    sCharacterSettings.mRightForceX
    , sCharacterSettings.mRightForceY);

  mRightActionAccumulator.Reset();
}

void PlayerControls::ExecuteActionLeft()
{
  if (!mLeftActionAccumulator.IsReady())
    return;

  mSimulatable->GetParticle().AddVelocity(
    sCharacterSettings.mLeftForceX
    , sCharacterSettings.mLeftForceY);

  mLeftActionAccumulator.Reset();
}

void PlayerControls::UpdateAccumulators(REAL delta)
{
  mUpActionAccumulator.Add(delta);
  mDownActionAccumulator.Add(delta);
  mRightActionAccumulator.Add(delta);
  mLeftActionAccumulator.Add(delta);
  mSpaceActionAccumulator.Add(MAX_DT);
}

void PlayerControls::ApplyInputs()
{
  if (mControls == CONTROLS_ARROWS)
  {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      ExecuteActionRight();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      ExecuteActionLeft();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && 
      // Check if player can jump
      abs(mSimulatable->GetParticle().GetAcceleration().y) < abs(mSimulatable->mGravity.y) + FLOAT_SMALL)
      ExecuteActionUp();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      ExecuteActionDown();
  }
  else
  {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      ExecuteActionRight();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      ExecuteActionLeft();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && 
      abs(mSimulatable->GetParticle().GetAcceleration().y) < abs(mSimulatable->mGravity.y) + FLOAT_SMALL)
      ExecuteActionUp();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      ExecuteActionDown();
  }
}
