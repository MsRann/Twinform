#include "Renderer.h"

#include <vector>
#include <iostream>
#include <thread>

#include "TwinformTypes.h"

#include <unordered_map>

namespace
{
	std::unordered_map<uint32_t, sf::Drawable*> mDrawables;
	std::list<sf::Drawable*> mParticles;
	// Members for measuring time passed in frames
	sf::Clock mClock;
	REAL mFrameTimeSec;
}

void Renderer::Add(uint32_t id, sf::Drawable& drawable)
{
	mDrawables[id] = &drawable;
}

void Renderer::AddParticle(sf::Drawable& drawable)
{
	mParticles.push_back(&drawable);
}

void Renderer::Remove(uint32_t id)
{
	mDrawables.erase(id);
}

void Renderer::RemoveParticle(sf::Drawable& drawable)
{
	mParticles.remove(&drawable);
}

REAL Renderer::GetLastFrametime()
{
	return mFrameTimeSec;
}

void Renderer::Render(sf::RenderWindow& window)
{
	sf::Time elapsed = mClock.restart();
	mFrameTimeSec = elapsed.asSeconds();

	window.clear();

	for (auto particle : mParticles)
	{
		if (particle != nullptr)
			window.draw(*particle);
	}

	for (auto drawable : mDrawables)
	{
		if (drawable.second != nullptr)
			window.draw(*drawable.second);
	}

	window.display();
}