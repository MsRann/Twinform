#include "Inventory.h"

namespace
{
  uint32_t sCollectibleCount = 0;
}

void Inventory::AddCollectible()
{
  ++sCollectibleCount;
}

uint32_t Inventory::GetCollectibleCount()
{
  return sCollectibleCount;
}