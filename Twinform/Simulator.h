#pragma once

#include "HashedCellStorage.h"
#include "TwinformTypes.h"
#include "Simulatable.h"
#include "Camera.h"

namespace Simulator
{
  HashedCellStorage& GetDynamicStorage();
  HashedCellStorage& GetStaticStorage();

  void Add(Simulatable& character);
  void DeleteDynamic(Simulatable& character);
  void DeleteStatic(Simulatable& character);
  void SetWindow(TwinformWindow* window);

  void Simulate(REAL delta);
  uint32_t GetId(const sf::Vector2f& position);
  uint32_t GetId(const sf::Vector2i& position);
}