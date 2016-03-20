#pragma once

#include <SFML/Graphics.hpp>
#include <stdint.h>

#include "TwinformTypes.h"

namespace Renderer
{
  void Add(uint32_t id, sf::Drawable& drawable);
  void AddParticle(sf::Drawable& drawable);
  void Remove(uint32_t id);
  void RemoveParticle(sf::Drawable& drawable);
  void Render(sf::RenderWindow& window);

  void DebugAddCollision(sf::FloatRect& rect);

  // Returns time(in milliseconds) of previous frame time
  REAL GetLastFrametime();
}