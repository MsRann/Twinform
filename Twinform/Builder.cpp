#ifdef BUILDING

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Builder.h"
#include "Renderer.h"
#include "TwinMath.h"
#include "Simulator.h"
#include "Creator.h"
#include "ActionTimeAccumulator.h"
#include "PlayerControls.h"

namespace
{
  sf::RectangleShape sCurrentDrawable;
  Camera* sCamera = nullptr;
  bool sCharactersSpawned = false;
  ActionTimeAccumulator sDeleteAccumulator(1.0f);
  ActionTimeAccumulator sSpawnAccumulator(100.0f);

  void ProcessMouseInputs();
  void ProcessKeyboardInputs();

  void ProcessMouseInputs()
  {
    if (!sCamera->GetWindow().hasFocus())
      return;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(sCamera->GetWindow());
    // TODO: Copy pasted code
    mousePosition.x += (int)(sCamera->GetView().getCenter().x 
      - ((REAL)sCamera->GetWindowWidth() / 2.0f));
    mousePosition.y += (int)(sCamera->GetView().getCenter().y 
      - ((REAL)sCamera->GetWindowHeight() / 2.0f));
    twinmath::SnapToGrid(mousePosition, GRID_WIDTH_HALF, GRID_HEIGHT_HALF);
    sCurrentDrawable.setPosition((REAL)mousePosition.x, (REAL)mousePosition.y);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      HashedCellStorage& staticStorage = Simulator::GetStaticStorage();
      if (!staticStorage.IsPointColliding(mousePosition))
      {
        Creator::MakeStaticGeometry(
          mousePosition
          , sf::Vector2f(static_cast<REAL>(GRID_WIDTH_HALF)
          , static_cast<REAL>(GRID_HEIGHT_HALF))
        );
      }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && 
        sDeleteAccumulator.IsReady())
    {
      uint32_t id = Simulator::GetId(mousePosition);
      Creator::Delete(id);
      sDeleteAccumulator.Reset();
    }
  }

  void ProcessKeyboardInputs()
  {
    sf::Vector2f offset;

    // Process screen offset based on WASD

    /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      offset += sf::Vector2f(0.05f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      offset += sf::Vector2f(-0.05f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      offset += sf::Vector2f(0.0f, -0.05f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      offset += sf::Vector2f(0.0f, 0.05f);*/

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
      // TODO: Copy pasted code
      sf::Vector2i mousePosition = sf::Mouse::getPosition(sCamera->GetWindow());
      mousePosition.x += 
        (int)(sCamera->GetView().getCenter().x
        - ((REAL)sCamera->GetWindowWidth() / 2.0f));
      mousePosition.y += 
        (int)(sCamera->GetView().getCenter().y
        - ((REAL)sCamera->GetWindowHeight() / 2.0f));

      twinmath::SnapToGrid(mousePosition, GRID_WIDTH_HALF, GRID_HEIGHT_HALF);

      HashedCellStorage& dynamicStorage = Simulator::GetDynamicStorage();
      if (!dynamicStorage.IsPointColliding(mousePosition))
      {
        sf::Vector2f positionf((float)mousePosition.x, (float)mousePosition.y);
        Creator::MakeCollectible(
          positionf
          , sf::Vector2f(static_cast<REAL>(GRID_WIDTH_HALF / 3)
          , static_cast<REAL>(GRID_HEIGHT_HALF / 3))
        );
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde) && sSpawnAccumulator.IsReady())
    {
      if (!Creator::GetPlayerNeutron())
      {
        Creator::MakeNeutron(
          new PlayerControls(CONTROLS_WASD)
          , sf::Vector2f(0, 300)
          , sf::Vector2f(25.0f, 25.0f)
          , sf::Vector2f(0.0f, 10.0f));
      }
      else
      {
        Creator::GetPlayerNeutron()->GetParticle().SetPosition(sf::Vector2f(0, 300));
      }
      sSpawnAccumulator.Reset();
    }

    sCamera->Translate(offset);
  }
}

void Builder::Initialize(Camera& twinformWindow)
{
  sCamera = &twinformWindow;
  sCurrentDrawable.setPosition(0.0f, 0.0f);
  sCurrentDrawable.setSize(sf::Vector2f(GRID_WIDTH / 2, GRID_HEIGHT / 2));
  sCurrentDrawable.setFillColor(sf::Color(0u, 0u, 0u, 0u));
  sCurrentDrawable.setOutlineThickness(2.0f);
  sCurrentDrawable.setOutlineColor(sf::Color(0u, 0u, 255u, 255u));
  Renderer::Add(0, sCurrentDrawable);
}

void Builder::Update(REAL delta)
{
  sSpawnAccumulator.Add(delta);
  sDeleteAccumulator.Add(delta);
  ProcessMouseInputs();
  ProcessKeyboardInputs();
}

void Builder::SetCharacterSpawned(bool value)
{
  sCharactersSpawned = value;
}

#endif