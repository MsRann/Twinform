#pragma once

#include <SFML/System/Vector2.hpp>

#include "TwinformTypes.h"

namespace twinmath
{
  REAL Dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
  void Normalize(sf::Vector2f& v);
  REAL Length(const sf::Vector2f& v);
  REAL LengthSquared(const sf::Vector2f& v);
  sf::Vector2f Lerp(const sf::Vector2f& v1, const sf::Vector2f& v2, REAL t);

  sf::Vector2i CreateGrid(const sf::Vector2f& v, unsigned int width, unsigned int height);
  sf::Vector2i CreateGrid(const sf::Vector2i& v, unsigned int width, unsigned int height);

  void SnapToGrid(sf::Vector2f& v, unsigned int gridWidth, unsigned int gridHeight);
  void SnapToGrid(sf::Vector2i& v, unsigned int gridWidth, unsigned int gridHeight);

  inline void Zero(sf::Vector2f v) { v.x = 0.0f; v.y = 0.0f; }
}