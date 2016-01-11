#pragma once
#include "Simulatable.h"

// TODO: 
// Make sure Static Geometry does not update its past array

class StaticGeometry : public Simulatable
{
public:
	StaticGeometry();
	StaticGeometry(sf::Vector2f startPosition, sf::Vector2f size, uint32_t id);
	~StaticGeometry();

	virtual void PreUpdate(REAL delta);
	sf::RectangleShape& GetDrawable();
private:
	sf::RectangleShape mDrawable;
};

