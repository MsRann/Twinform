#pragma once

#include <SFML/Graphics.hpp>

typedef float REAL;

static const REAL MAX_DT = 0.01f;
static const REAL MASS_MAX = -1.0f;
static const REAL FLOAT_SMALL = 0.001f;
static const REAL INVERSE_MASS_SMALL = 0.000001f;

// Number of commands consumed per physics update
static const unsigned int COMMAND_CONSUMPTION = 1;

static const unsigned int GRID_WIDTH = 50;
static const unsigned int GRID_HEIGHT = 50;

static const unsigned int GRID_WIDTH_HALF = GRID_WIDTH / 2;
static const unsigned int GRID_HEIGHT_HALF = GRID_HEIGHT / 2;

enum PhysicalFlags
{
  HAS_GRAVITY = 0x1,
  STATIC_GEOMETRY = 0x2,
  DONT_INTEGRATE = 0x4,
  CAN_PICKUP = 0x8,
};

enum TwinformObject 
{
  STATIC, 
  CONTROLLABLE,
  COLLECTIBLE,
  UNDETERMINABLE
};