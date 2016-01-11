#include "Simulator.h"

#include <iostream>
#include <vector>

#include "HashedCellStorage.h"
#include "TwinMath.h"

using namespace twinmath;

namespace
{
	std::vector<Simulatable*> mCharacters;
	// Split up dynamic and static storage into its own maps.
	// Dynamic storage needs to be updated each physics update(since things are moving).
	HashedCellStorage mDynamicStorage;
	HashedCellStorage mStaticStorage;
	TwinformWindow* mWindow = nullptr;

	// Private functions
	void ProcessCollisions();
	void HandleStaticCollisions(Simulatable*& character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& staticCollisions);
	void HandleDynamicCollisions(Simulatable*& character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& dynamicCollisions);
	void FixCharacterParticle(Simulatable*& character, const sf::FloatRect& collision, const sf::Vector2f& hitVelocity, const sf::Vector2f& hitPosition, const sf::Vector2f& hitAcceleration);

	void ProcessCollisions(Simulatable*& character)
	{
		std::vector<std::pair<Simulatable*, sf::FloatRect>> staticCollisions;
		if (mStaticStorage.IsColliding(*character, staticCollisions))
			HandleStaticCollisions(character, staticCollisions);

		std::vector<std::pair<Simulatable*, sf::FloatRect>> dynamicCollisions;
		if (mDynamicStorage.IsColliding(*character, dynamicCollisions))
			HandleStaticCollisions(character, dynamicCollisions);
	}

	void HandleStaticCollisions(Simulatable*& character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& staticCollisions)
	{
		for (auto collision : staticCollisions)
		{
			Particle& characterParticle = character->GetParticle();
			RingBuffer<Particle>& past = character->GetPast();
			unsigned int rewind = 1;
			// TODO: Check how many times this runs
			int i = 0; 
			while (character->GetCollisionBounds().intersects(collision.second))
			{
				++i;
				Particle pastParticle;
				// Go back in time and find a position in which the simulatable wasn't colliding
				if (past.Get(rewind++, pastParticle))
				{
					//std::cout << "FIXING: " << i++ << std::endl;
					sf::Vector2f hitVelocity = character->GetVelocity();
					sf::Vector2f hitPosition = character->GetPosition();
					sf::Vector2f hitAcceleration = character->GetAcceleration();
					character->SetParticle(pastParticle);
					character->UpdateCollisionBounds();
					characterParticle.SetAcceleration(0.0f, 0.0f);
					FixCharacterParticle(character, collision.second, hitVelocity, hitPosition, hitAcceleration);
				}
				else
				{
					break;
				}
			}
		}
	}

	void HandleDynamicCollisions(Simulatable*& character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& dynamicCollisions)
	{

	}

	// This is a FUGLY function and I THINK will only work when correction can line up appropriately with the 
	// x-axis or y-axis. It will need work to fix for both.
	// TODO: This function causes characters to go through walls, that will need to be fixed.
	//	REPRO: Run and jump into a vertical wall and keep holding the direction through the wall, eventually you'll go through.
	//  REPRO RATE: About 20%, as an estimate. It doesn't happen often.
	void FixCharacterParticle(Simulatable*& character, const sf::FloatRect& collision, const sf::Vector2f& hitVelocity, const sf::Vector2f& hitPosition, const sf::Vector2f& hitAcceleration)
	{
		// The point of this function will be to soften velocity on the x and y axis until an integration step
		// will no longer cause collision. After that is done move the character once witih this fixed velocity.
		Particle safeParticle = character->GetParticleCopy();
		sf::Vector2f originalVelocity = character->GetVelocity();
		sf::Vector2f position = character->GetPosition();
		Particle originalParticle = character->GetParticleCopy();
		Particle& characterParticle = character->GetParticle();

		characterParticle.SetVelocity(hitVelocity.x, 0.0f);
		characterParticle.SetAcceleration(hitAcceleration.x, 0.0f);
		characterParticle.SetPosition(hitPosition.x, position.y);

		// Fixed delta time
		//characterParticle.Integrate(MAX_DT);
		character->UpdateCollisionBounds();

		if (!character->GetCollisionBounds().intersects(collision))
			return;

		characterParticle = originalParticle;
		characterParticle.SetVelocity(0.0f, hitVelocity.y);
		characterParticle.SetAcceleration(0.0f, hitAcceleration.y);
		characterParticle.SetPosition(position.x, hitPosition.y);

		//characterParticle.Integrate(MAX_DT);
		character->UpdateCollisionBounds();

		if (!character->GetCollisionBounds().intersects(collision))
			return;

		character->SetParticle(safeParticle);
		character->UpdateCollisionBounds();
	}
}


HashedCellStorage& Simulator::GetDynamicStorage() 
{ 
	return mDynamicStorage; 
}

HashedCellStorage& Simulator::GetStaticStorage() 
{ 
	return mStaticStorage; 
}

void Simulator::Add(Simulatable& character)
{
	// Insert character into either dynamic or static storage based on its falgs
	if (character.GetFlags() & STATIC_GEOMETRY)
	{
		// If the objects collision bounds are larger than a tile we have a special insertion case
		sf::FloatRect collisionRect = character.GetCollisionBounds();

		// I'm not sure if it's possible to have a negative width, I should look it up someday
		if (collisionRect.width > GRID_WIDTH)
		{
			float width = collisionRect.width;
			while ((width -= GRID_WIDTH) > 0)
				mStaticStorage.InsertToKey(character, sf::Vector2i((int)floor((collisionRect.left + width) / GRID_WIDTH), (int)floor(collisionRect.top / GRID_HEIGHT)));
		}

		if (collisionRect.height > GRID_HEIGHT)
		{
			float height = collisionRect.height;
			while ((height -= GRID_HEIGHT) > 0)
				mStaticStorage.InsertToKey(character, sf::Vector2i((int)floor(collisionRect.left / GRID_WIDTH), (int)floor((collisionRect.top + height) / GRID_HEIGHT)));
		}

		mStaticStorage.Insert(character);
	}
	else
	{	
		// TODO: When dynamic storage becomes bigger than grid bounds update this to consider that
		mDynamicStorage.Insert(character);
	}

	mCharacters.push_back(&character);
}

void Simulator::DeleteDynamic(Simulatable& character)
{
	mDynamicStorage.Remove(character);
	mCharacters.erase(std::find(mCharacters.begin(), mCharacters.end(), &character), mCharacters.end());
}

void Simulator::DeleteStatic(Simulatable& character)
{
	mStaticStorage.Remove(character);
	mCharacters.erase(std::find(mCharacters.begin(), mCharacters.end(), &character), mCharacters.end());
}

void Simulator::SetWindow(TwinformWindow* window)
{
	mWindow = window;
}

void Simulator::Simulate(REAL delta)
{
	mDynamicStorage.UpdateCells();

	// Order of operations for each character:
	// 1. Prepare Update
	// 2. Pre Update
	// 3. Post Update
	// 4. Process Collisions

	for (auto character : mCharacters)
	{
		// Don't apply inputs to characters if the window does not have focus
		if (mWindow && !mWindow->window->hasFocus())
			break;
		
		character->PrepareUpdate(delta);
	}

	for (auto character : mCharacters)
	{
		// Only check collision and update things that are moving
		if (character->GetFlags() & STATIC_GEOMETRY)
			continue;

		if (character->GetFlags() & DONT_INTEGRATE)
			continue;

		// Pre update will set up variables and is usually overwritten by a derived class
		character->PreUpdate(delta);
	}

	for (auto character : mCharacters)
	{
		// Only check collision and update things that are moving
		if (character->GetFlags() & STATIC_GEOMETRY)
			continue;

		if (character->GetFlags() & DONT_INTEGRATE)
			continue;

		character->PostUpdate(delta);
	}

	for (auto character : mCharacters)
	{
		// Only check collision and update things that are moving
		if (character->GetFlags() & STATIC_GEOMETRY)
			continue;

		if (character->GetFlags() & DONT_INTEGRATE)
			continue;

		ProcessCollisions(character);
	}
}

uint32_t Simulator::GetId(const sf::Vector2f& position)
{
	Simulatable* sim = nullptr;

	mStaticStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	mDynamicStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	return 0;
}

uint32_t Simulator::GetId(const sf::Vector2i& position)
{
	Simulatable* sim = nullptr;

	mStaticStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	mDynamicStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	return 0;
}