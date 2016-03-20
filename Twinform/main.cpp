#include <SFML/Graphics.hpp>

#include <iostream>

#include "Renderer.h"
#include "Simulator.h"
#include "Creator.h"
#include "StaticGeometry.h"
#include "TwinMath.h"
#include "Camera.h"
#include "PropertyReader.h"
#include "ParticleSystem.h"
#include "Terminal.h"
#include "CommandStream.h"
#include "Settings.h"

#ifdef BUILDING
#include "Builder.h"
#endif

int main()
{
  // Read all settings from file
  Settings::Load();

  const Settings::WindowSettings& windowSettings = Settings::GetWindowSettings();
  
  sf::ContextSettings settings;
  settings.antialiasingLevel = 2;
  
  sf::RenderWindow window(
    sf::VideoMode(
      windowSettings.mWindowWidth
      , windowSettings.mWindowHeight)
    , windowSettings.mWindowTitle
    , windowSettings.mWindowStyle
    , settings);

  sf::View view;
  view.reset(sf::FloatRect(0
    , 0
    , static_cast<float>(windowSettings.mWindowWidth)
    , static_cast<float>(windowSettings.mWindowHeight)));

  view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

  window.setView(view);

  // Cache window and view because it will be used in many places and I don't want to make it global
  // but making making a static window and view would be better.
  TwinformWindow twinformWindow;
  twinformWindow.window = &window;
  twinformWindow.view = &view;

  Simulator::SetWindow(&twinformWindow);

  Camera camera(twinformWindow
    , windowSettings.mWindowWidth
    , windowSettings.mWindowHeight);

  sf::Vector2f start(0.0f, 360.0f);
  twinmath::SnapToGrid(start, GRID_WIDTH_HALF, GRID_HEIGHT_HALF);
  Creator::MakeStaticGeometry(start, sf::Vector2f((REAL)GRID_WIDTH_HALF, (REAL)GRID_HEIGHT_HALF));

  REAL accumulator = 0.0f;
  REAL t = 0.0f;

  // TODO: Finish implementing well behaved physics steps with 
  // fix your timestep article. This could cause temporal aliasing but 
  // I'm not sure why :(

  // Building macro is just for map building. So undefine it before making a distributable build
#ifdef BUILDING
  Builder::Initialize(camera);
#endif

  while (window.isOpen())
  {
    REAL time = Renderer::GetLastFrametime();
    accumulator += time;
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

#ifdef BUILDING
    Builder::Update(MAX_DT);
#endif

    while (accumulator >= MAX_DT)
    {
      Simulator::Simulate(MAX_DT);
      ParticleSystem::Update(MAX_DT);
      // Execute one command per iteration
      CommandStream::Execute(COMMAND_CONSUMPTION);
      accumulator -= MAX_DT;
      t += MAX_DT;
    }

    // For some reason the camera moves to player here, seems like a terrible spot for this code
    if (Creator::GetPlayer())
    {
      camera.MoveTo(Creator::GetPlayer()->GetParticle().GetPosition());
    }

    Renderer::Render(window);
  }

  Terminal::Kill();

  return 0;
}