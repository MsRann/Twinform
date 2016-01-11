#pragma once

#include <SFML/Graphics.hpp>

#include "TwinformTypes.h"

namespace ParticleSystem
{
	class Particle
	{
	public:
		REAL mTimeToLive;
		REAL mTimeAlive;
		sf::RectangleShape mDrawable;
	};

	void Create(const sf::Vector2f& position);
	void Update(REAL delta);
};