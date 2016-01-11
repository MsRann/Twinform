#include "ParticleSystem.h"
#include "Renderer.h"

#include <list>

namespace
{
	std::list<ParticleSystem::Particle> mParticles;
};

void ParticleSystem::Create(const sf::Vector2f& position)
{
	REAL size = 8.0f;
	mParticles.push_back(ParticleSystem::Particle());
	ParticleSystem::Particle& particle = mParticles.back();
	particle.mDrawable = sf::RectangleShape(sf::Vector2f(size, size));
	particle.mDrawable.setFillColor(sf::Color(0, 255, 255, 150));
	particle.mDrawable.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));
	particle.mTimeToLive = 1.5f;
	particle.mDrawable.setPosition(position);
	particle.mTimeAlive = 0.0f;
	Renderer::AddParticle(particle.mDrawable);
}

void ParticleSystem::Update(REAL delta)
{
	std::list<ParticleSystem::Particle>::iterator particle = mParticles.begin();
	while (particle != mParticles.end())
	{
		if (particle->mTimeAlive >= particle->mTimeToLive)
		{
			Renderer::RemoveParticle(particle->mDrawable);
			mParticles.erase(particle++);
		}
		else
		{
			particle->mDrawable.rotate(5.0f);
			particle->mTimeAlive += delta;
			++particle;
		}
	}
}
