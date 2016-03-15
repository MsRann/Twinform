#pragma once

#include "Simulatable.h"

class Collectible : public Simulatable
{
public:
	Collectible();
	Collectible(sf::Vector2f start, sf::Vector2f size, uint32_t id);
	~Collectible();

	virtual void PreUpdate(REAL delta);
	virtual void PostUpdate(REAL delta);

	sf::RectangleShape& GetDrawable();

private:
	sf::RectangleShape mDrawable;
};

