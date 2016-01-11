#include "TwinMath.h"

#include <math.h>

REAL twinmath::Dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.x * v2.x;
}

void twinmath::Normalize(sf::Vector2f& v)
{
	REAL len = Length(v);
	v.x /= len;
	v.y /= len;
}

REAL twinmath::Length(const sf::Vector2f& v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

REAL twinmath::LengthSquared(const sf::Vector2f& v)
{
	return pow((v.x * v.x + v.y * v.y), 2);
}

sf::Vector2i twinmath::CreateGrid(const sf::Vector2f& v, unsigned int width, unsigned int height)
{
	return sf::Vector2i((int)(v.x / width), (int)(v.y / height));
}

sf::Vector2i twinmath::CreateGrid(const sf::Vector2i& v, unsigned int width, unsigned int height)
{
	return sf::Vector2i(v.x / width, v.y / height);
}

void twinmath::SnapToGrid(sf::Vector2f& v, unsigned int gridWidth, unsigned int gridHeight)
{
	sf::Vector2i grid = twinmath::CreateGrid(v, gridWidth, gridHeight);
	v.x = (REAL)grid.x * gridWidth;
	v.y = (REAL)grid.y * gridHeight;
}

void twinmath::SnapToGrid(sf::Vector2i& v, unsigned int gridWidth, unsigned int gridHeight)
{
	sf::Vector2i grid = twinmath::CreateGrid(v, gridWidth, gridHeight);
	v.x = grid.x * gridWidth;
	v.y = grid.y * gridHeight;
}

