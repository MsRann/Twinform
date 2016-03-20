#include "Camera.h"


Camera::Camera(TwinformWindow& window, unsigned int width, unsigned int height) :
  mWindow(*window.window)
  ,mView(*window.view)
  ,mWindowWidth(width)
  ,mWindowHeight(height)
{
}


Camera::~Camera()
{
}

void Camera::MoveTo(const sf::Vector2f& position)
{
  mView.setCenter(position);
  mWindow.setView(mView);
}

void Camera::Translate(const sf::Vector2f& offset)
{
  mView.move(offset);
  mWindow.setView(mView);
}

unsigned int Camera::GetWindowWidth() const
{
  return mWindowWidth;
}

unsigned int Camera::GetWindowHeight() const
{
  return mWindowHeight;
}

const sf::RenderWindow& Camera::GetWindow() const
{
  return mWindow;
}

const sf::View& Camera::GetView() const
{
  return mView;
}