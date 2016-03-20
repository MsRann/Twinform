#pragma once

#include <vector>

#include "TwinformTypes.h"
#include "ControllableCharacter.h"
#include "Collectible.h"
#include "StaticGeometry.h"

namespace Creator
{
  StaticGeometry* MakeStaticGeometry(const sf::Vector2f& position, const sf::Vector2f& size);
  StaticGeometry* MakeStaticGeometry(const sf::Vector2i& position, const sf::Vector2f& size);
  StaticGeometry* MakeStaticGeometryFromSize(const sf::Vector2i& position, const sf::Vector2f& size);
  ControllableCharacter* MakeControllableCharacter(const sf::Vector2f& start, const sf::Vector2f& size, ControllableControls controls, const sf::Vector2f& gravity);
  Collectible* MakeCollectible(const sf::Vector2f& start, const sf::Vector2f& size);
  ControllableCharacter* GetPlayer();

  void Save(const std::string& filename);
  void Load(const std::string& filename);

  // Will delete the object with some id
  void Delete(const uint32_t& id);
  void Clear();

  TwinformObject GetType(const uint32_t& id);
}