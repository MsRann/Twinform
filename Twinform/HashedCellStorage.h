#pragma once

#include <unordered_map>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Simulatable.h"

struct CellHash
{
  std::size_t operator()(const sf::Vector2i& location) const
  {
    // Pg. 288 of Real-Time Collision Detection by Christer Ericson gives
    // this hash and says it may not be very strong. So maybe look up a different
    // hash function someday.
    const int h1 = 0x8da6b343;
    const int h2 = 0xd8163841;
    size_t n = h1 * location.x + h2 * location.y;
    return n;
  }
};

class HashedCellStorage
{
public:
  HashedCellStorage();
  ~HashedCellStorage();

  void Insert(Simulatable& simulatable);
  void InsertToKey(Simulatable& simulatable, sf::Vector2i key);
  void Remove(Simulatable& simulatable);
  void UpdateCells();

  bool IsColliding(Simulatable& simulatable);
  bool IsColliding(Simulatable& simulatable, std::vector<std::pair<Simulatable*, sf::FloatRect>>& simulatablesHit);

  bool IsPointColliding(sf::Vector2f point, Simulatable*& simulatableHit);
  bool IsPointColliding(sf::Vector2i point, Simulatable*& simulatableHit);

  bool IsPointColliding(sf::Vector2f point);
  bool IsPointColliding(sf::Vector2i point);

private:
  sf::Vector2i CreateKey(const Simulatable& sim) const;
  sf::Vector2i CreateKey(const sf::Vector2f& pos) const;
  sf::Vector2i CreateKey(const sf::Vector2i& pos) const;

  typedef std::unordered_map<sf::Vector2i, std::vector<Simulatable*>, CellHash> CollisionMap;
  CollisionMap mGrid;
};

