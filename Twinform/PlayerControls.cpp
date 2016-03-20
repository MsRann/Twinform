#include "PlayerControls.h"

#include "PropertyReader.h"

namespace
{
  static PropertyReader sCharacterSettings("Settings/character_settings.txt");

  static bool sSettingsLoaded = false;
  static REAL sRightForceX;
  static REAL sRightForceY;

  static REAL sLeftForceX;
  static REAL sLeftForceY;

  static REAL sJumpForceX;
  static REAL sJumpForceY;

  static REAL sDownForceX;
  static REAL sDownForceY;

  static REAL sRadius;

  static void LoadSettings();

  void LoadSettings()
  {
    if (sSettingsLoaded)
      return;

    sCharacterSettings.ReadFloat("RightForceX", sRightForceX);
    sCharacterSettings.ReadFloat("RightForceY", sRightForceY);
    sCharacterSettings.ReadFloat("LeftForceX", sLeftForceX);
    sCharacterSettings.ReadFloat("LeftForceY", sLeftForceY);
    sCharacterSettings.ReadFloat("JumpForceX", sJumpForceX);
    sCharacterSettings.ReadFloat("JumpForceY", sJumpForceY);
    sCharacterSettings.ReadFloat("DownForceX", sDownForceX);
    sCharacterSettings.ReadFloat("DownForceY", sDownForceY);
    sCharacterSettings.ReadFloat("Radius", sRadius);
    sSettingsLoaded = true;
  }
}

PlayerControls::PlayerControls(ControllableControls controls) :
  mControls(controls)
  , mUpActionAccumulator(0.50f)
  , mRightActionAccumulator(0.05f)
  , mLeftActionAccumulator(0.05f)
  , mDownActionAccumulator(0.50f)
  , mSpaceActionAccumulator(MAX_DT)
{
  LoadSettings();
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

  //mSimulatable->GetParticle().AddForce(sf::Vector2f(sDownForceX, sDownForceY));
  //mDownActionAccumulator.Reset();
}

void PlayerControls::ExecuteActionUp()
{
  if (!mUpActionAccumulator.IsReady())
    return;

  float jumpForce = sJumpForceY;
  if (mSimulatable->mGravity.y < 0.0f)
    jumpForce *= -1;

  mSimulatable->GetParticle().AddForce(sf::Vector2f(sJumpForceX, jumpForce));
  mUpActionAccumulator.Reset();
}

void PlayerControls::ExecuteActionRight()
{
  if (!mRightActionAccumulator.IsReady())
    return;

  mSimulatable->GetParticle().AddVelocity(sRightForceX, sRightForceY);
  mRightActionAccumulator.Reset();
}

void PlayerControls::ExecuteActionLeft()
{
  if (!mLeftActionAccumulator.IsReady())
    return;

  mSimulatable->GetParticle().AddVelocity(sLeftForceX, sLeftForceY);
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
