#include "Settings.h"

#include <cassert>

#include "PropertyReader.h"

namespace
{
  using namespace Settings;
  
  static const std::string sCharacterLoc = "Settings/character_settings.txt";
  static const std::string sWindowLoc = "Settings/window_settings.txt";

  static CharacterSettings sCharacterSettings;
  static WindowSettings sWindowSettings;

  static bool sSettingsLoaded = false;
}

void Settings::Load()
{
  if (sSettingsLoaded) return;

  PropertyReader settings(sCharacterLoc);

  settings.ReadFloat("RightForceX", sCharacterSettings.mRightForceX);
  settings.ReadFloat("RightForceY", sCharacterSettings.mRightForceY);
  settings.ReadFloat("LeftForceX", sCharacterSettings.mLeftForceX);
  settings.ReadFloat("LeftForceY", sCharacterSettings.mLeftForceY);
  settings.ReadFloat("JumpForceX", sCharacterSettings.mJumpForceX);
  settings.ReadFloat("JumpForceY", sCharacterSettings.mJumpForceY);
  settings.ReadFloat("DownForceX", sCharacterSettings.mDownForceX);
  settings.ReadFloat("DownForceY", sCharacterSettings.mDownForceY);
  settings.ReadFloat("Radius", sCharacterSettings.mRadius);
  // Just in case names conflict in setting files clear the memory and reload
  settings.Free();

  settings.LoadFile(sWindowLoc);
  settings.ReadInt("WindowWidth", sWindowSettings.mWindowWidth);
  settings.ReadInt("WindowHeight", sWindowSettings.mWindowHeight);
  settings.ReadString("WindowTitle", sWindowSettings.mWindowTitle);
  settings.ReadInt("WindowStyle", sWindowSettings.mWindowStyle);

  // Set settings as loaded
  sSettingsLoaded = true;
}

const Settings::CharacterSettings& Settings::GetCharacterSettings()
{
  assert(sSettingsLoaded);
  return sCharacterSettings;
}

const Settings::WindowSettings& Settings::GetWindowSettings()
{
  assert(sSettingsLoaded);
  return sWindowSettings;
}
