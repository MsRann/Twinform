#pragma once

#include "TwinformTypes.h"

struct TwinformWindow
{
  sf::RenderWindow* window;
  sf::View* view;
};

class Camera
{
public:
  Camera(TwinformWindow& window, unsigned int width, unsigned int height);
  ~Camera();
  // C++11 explicit forbid
  Camera(const Camera& copy) = delete;

  void MoveTo(const sf::Vector2f& position);
  void Translate(const sf::Vector2f& offset);

  unsigned int GetWindowWidth() const;
  unsigned int GetWindowHeight() const;

  const sf::RenderWindow& GetWindow() const;
  const sf::View& GetView() const;

private:
  sf::RenderWindow& mWindow;
  sf::View& mView;

  unsigned int mWindowWidth;
  unsigned int mWindowHeight;
};

