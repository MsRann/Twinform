#include "Renderer.h"

#include <vector>
#include <iostream>
#include <thread>

#include "TwinformTypes.h"

#include <unordered_map>

namespace
{
  std::unordered_map<uint32_t, sf::Drawable*> sUniqueDrawables;
  std::list<sf::Drawable*> sParticles;
  // Only for debug
  std::list<sf::FloatRect*> sCollisionData;
  sf::RectangleShape sDbgRectangle;
  // Members for measuring time passed in frames
  sf::Clock sClock;
  REAL sFrameTimeSec;
}

void Renderer::Add(uint32_t id, sf::Drawable& drawable)
{
  sUniqueDrawables[id] = &drawable;
}

void Renderer::AddParticle(sf::Drawable& drawable)
{
  sParticles.push_back(&drawable);
}

void Renderer::Remove(uint32_t id)
{
  sUniqueDrawables.erase(id);
}

void Renderer::RemoveParticle(sf::Drawable& drawable)
{
  sParticles.remove(&drawable);
}

REAL Renderer::GetLastFrametime()
{
  return sFrameTimeSec;
}

void Renderer::Render(sf::RenderWindow& window)
{
  sf::Time elapsed = sClock.restart();
  sFrameTimeSec = elapsed.asSeconds();

  window.clear();

  for (auto particle : sParticles)
  {
    if (particle != nullptr)
      window.draw(*particle);
  }

  for (auto drawable : sUniqueDrawables)
  {
    if (drawable.second != nullptr)
      window.draw(*drawable.second);
  }

  for (auto rect : sCollisionData)
  {
    sDbgRectangle.setSize(sf::Vector2f(rect->width, rect->height));
    sDbgRectangle.setOutlineColor(sf::Color::Red);
    sDbgRectangle.setFillColor(sf::Color(0, 0, 0, 0));
    sDbgRectangle.setOutlineThickness(1.0f);
    sDbgRectangle.setPosition(sf::Vector2f(rect->left, rect->top));
    window.draw(sDbgRectangle);
  }

  window.display();
}

void Renderer::DebugAddCollision(sf::FloatRect& rect)
{
  sCollisionData.push_back(&rect);
}