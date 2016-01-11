#pragma once

#include "Particle.h"
#include "TwinformTypes.h"
#include "RingBuffer.h"

#include <SFML\Graphics\RectangleShape.hpp>
#include <stdint.h>

class Simulatable
{
public:
	Simulatable();
	Simulatable(sf::Vector2f start, sf::Vector2f size);
	~Simulatable();

	// This will ALWAYS run before updates, so only put things that, don't need to be ever paused, in here
	virtual void PrepareUpdate(REAL delta) {};
	// Pre Update will be overriden by base classes to add custom functionality
	virtual void PreUpdate(REAL delta) = 0;
	// Post update will be done for every character and is primarily responsible for integration
	virtual void PostUpdate(REAL delta);

	void UpdateCollisionBounds();

	void SetGravity(const sf::Vector2f& gravity);

	void SetPrevKey(const sf::Vector2i& key);
	sf::Vector2i GetPrevKey() const;

	void SetParticle(const Particle& particle);

	const sf::FloatRect& GetCollisionBounds() const;

	const sf::Vector2f& GetPosition() const;
	const sf::Vector2f& GetVelocity() const;
	const sf::Vector2f& GetAcceleration() const;
	const sf::Vector2f& GetGravity() const;

	void SetFlag(PhysicalFlags flag);
	void UnsetFlag(PhysicalFlags flag);

	Particle& GetParticle();
	Particle GetParticleCopy();

	unsigned int GetFlags() const;
	REAL GetMass() const;

	RingBuffer<Particle>& GetPast();
	uint32_t GetID();
protected:
	Particle mParticle;
	sf::Vector2f mGravity;
	sf::Vector2i mPrevKey;

	uint32_t mFlags;

	sf::FloatRect mCollisionBounds;

	RingBuffer<Particle> mPast;
	uint32_t mID;
};

