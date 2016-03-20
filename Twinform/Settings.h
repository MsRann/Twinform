#pragma once

#include <string>

#include "TwinformTypes.h"

namespace Settings
{
  struct CharacterSettings
  {
    CharacterSettings() :
      mRightForceX(0.0)
      , mRightForceY(0.0)
      , mLeftForceX(0.0)
      , mLeftForceY(0.0)
      , mJumpForceX(0.0)
      , mJumpForceY(0.0)
      , mDownForceX(0.0)
      , mDownForceY(0.0)
      , mRadius(0.0) {};

    REAL mRightForceX;
    REAL mRightForceY;
    REAL mLeftForceX;
    REAL mLeftForceY;
    REAL mJumpForceX;
    REAL mJumpForceY;
    REAL mDownForceX;
    REAL mDownForceY;
    REAL mRadius;
  };

  struct WindowSettings
  {
    WindowSettings() :
      mWindowWidth(0)
      , mWindowHeight(0)
      , mWindowTitle()
      , mWindowStyle(0) {};

    int mWindowWidth;
    int mWindowHeight;
    std::string mWindowTitle;
    int mWindowStyle;
  };

  // Loads all gam settings
  void Load();
  // Get character settings
  const CharacterSettings& GetCharacterSettings();
  // Get window settings
  const WindowSettings& GetWindowSettings();
}