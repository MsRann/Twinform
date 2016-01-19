#include "Simulator.h"

#include <iostream>
#include <vector>

#include "HashedCellStorage.h"
#include "TwinMath.h"
#include "Debug.h"

using namespace twinmath;

namespace
{
	std::vector<Simulatable*> sCharacters;
	// Split up dynamic and static storage into its own maps.
	// Dynamic storage needs to be updated each physics update(since things are moving).
	HashedCellStorage sDynamicStorage;
	HashedCellStorage sStaticStorage;
	TwinformWindow* sWindow = nullptr;

	// Private functions
	void ProcessCollisions();
	void HandleStaticCollisions(Simulatable* character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& staticCollisions);
	void HandleDynamicCollisions(Simulatable* character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& dynamicCollisions);
	void FixCharacterParticle(Simulatable* character, const sf::FloatRect& collision, const sf::Vector2f& hitVelocity, const sf::Vector2f& hitPosition, const sf::Vector2f& hitAcceleration);
	sf::Vector2f CalculateIntersectionNormal(const sf::FloatRect& a, const sf::FloatRect& b);

	void ProcessCollisions(Simulatable* character)
	{
		std::vector<std::pair<Simulatable*, sf::FloatRect>> staticCollisions;
		if (sStaticStorage.IsColliding(*character, staticCollisions))
			HandleStaticCollisions(character, staticCollisions);

		std::vector<std::pair<Simulatable*, sf::FloatRect>> dynamicCollisions;
		if (sDynamicStorage.IsColliding(*character, dynamicCollisions))
			HandleStaticCollisions(character, dynamicCollisions);
	}

	void HandleStaticCollisions(Simulatable* character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& staticCollisions)
	{
		for (auto collision : staticCollisions)
		{
			Particle& characterParticle = character->GetParticle();
			RingBuffer<Particle>& past = character->GetPast();
			unsigned int rewind = 1;
			while (character->GetCollisionBounds().intersects(collision.second))
			{
				Particle pastParticle;
				// Go back in time and find a position in which the simulatable wasn't colliding
				if (past.Get(rewind++, pastParticle) && !character->EstimateCollisionBounds(pastParticle.GetPosition()).intersects(collision.second))
				{
					sf::Vector2f hitVelocity = character->GetParticle().GetVelocity();
					sf::Vector2f hitPosition = character->GetParticle().GetPosition();
					sf::Vector2f hitAcceleration = character->GetParticle().GetAcceleration();
					character->SetParticle(pastParticle);
					character->UpdateCollisionBounds();
					characterParticle.SetAcceleration(0.0f, 0.0f);
					FixCharacterParticle(character, collision.second, hitVelocity, hitPosition, hitAcceleration);
				}
			}
		}
	}

	void HandleDynamicCollisions(Simulatable* character, std::vector<std::pair<Simulatable*, sf::FloatRect>>& dynamicCollisions)
	{
		HandleStaticCollisions(character, dynamicCollisions);
	}

	// This is a confusing way to fix collision.
	// Basically what happens is the following.
	// 1. All the adjacent collisions are detected and added to staticCollisions.
	// 2. For each collision.
	//    - Rewind time to a position and velocity/acceleration where the character was not colliding
	// 3. Fix the character particle
	//    - This means reapplying the x or y attributes AT COLLISION
	void FixCharacterParticle(Simulatable* character, const sf::FloatRect& collision, const sf::Vector2f& hitVelocity, const sf::Vector2f& hitPosition, const sf::Vector2f& hitAcceleration)
	{
		// The point of this function will be to soften velocity on the x and y axis until an integration step
		// will no longer cause collision. After that is done move the character once witih this fixed velocity.
		sf::Vector2f position = character->GetParticle().mPosition;
		Particle originalParticle = character->GetParticleCopy();
		Particle& characterParticle = character->GetParticle();

		characterParticle.SetVelocity(hitVelocity.x, 0.0f);
		characterParticle.SetAcceleration(hitAcceleration.x, 0.0f);
		characterParticle.SetPosition(hitPosition.x, position.y);
		character->UpdateCollisionBounds();

		if (!character->GetCollisionBounds().intersects(collision))
			return;

		characterParticle = originalParticle;
		characterParticle.SetVelocity(0.0f, hitVelocity.y);
		characterParticle.SetAcceleration(0.0f, hitAcceleration.y);
		characterParticle.SetPosition(position.x, hitPosition.y);
		character->UpdateCollisionBounds();
	}
}

HashedCellStorage& Simulator::GetDynamicStorage() 
{ 
	return sDynamicStorage; 
}

HashedCellStorage& Simulator::GetStaticStorage() 
{ 
	return sStaticStorage; 
}
//#include "Renderer.h"
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
				sStaticStorage.InsertToKey(character, sf::Vector2i((int)floor((collisionRect.left + width) / GRID_WIDTH), (int)floor(collisionRect.top / GRID_HEIGHT)));
		}

		if (collisionRect.height > GRID_HEIGHT)
		{
			float height = collisionRect.height;
			while ((height -= GRID_HEIGHT) > 0)
				sStaticStorage.InsertToKey(character, sf::Vector2i((int)floor(collisionRect.left / GRID_WIDTH), (int)floor((collisionRect.top + height) / GRID_HEIGHT)));
		}

		sStaticStorage.Insert(character);
	}
	else
	{	
		// TODO: When dynamic storage becomes bigger than grid bounds update this to consider that
		sDynamicStorage.Insert(character);
	}
	//Renderer::DebugAddCollision(character.mCollisionBounds);
	sCharacters.push_back(&character);
}

void Simulator::DeleteDynamic(Simulatable& character)
{
	sDynamicStorage.Remove(character);
	sCharacters.erase(std::find(sCharacters.begin(), sCharacters.end(), &character), sCharacters.end());
}

void Simulator::DeleteStatic(Simulatable& character)
{
	sStaticStorage.Remove(character);
	sCharacters.erase(std::find(sCharacters.begin(), sCharacters.end(), &character), sCharacters.end());
}

void Simulator::SetWindow(TwinformWindow* window)
{
	sWindow = window;
}

void Simulator::Simulate(REAL delta)
{
	sDynamicStorage.UpdateCells();

	// This looks dumb but it's just to assure that updates per simulation step happen in a guaranteed order.
	// All characters do a prepareUpdate first then a preUpdate, etc etc. So no characters PostUpdate will occur
	// before every characters Prepare and Pre Updates are done

	for (auto character : sCharacters)
	{
		// Don't apply inputs to characters if the window does not have focus
		if (sWindow && !sWindow->window->hasFocus())
			break;
		
		character->PrepareUpdate(delta);
	}

	for (auto character : sCharacters)
	{
		// Only check collision and update things that are moving
		if (character->GetFlags() & STATIC_GEOMETRY)
			continue;

		if (character->GetFlags() & DONT_INTEGRATE)
			continue;

		// Pre update will set up variables and is usually overwritten by a derived class
		character->PreUpdate(delta);
	}

	for (auto character : sCharacters)
	{
		// Only check collision and update things that are moving
		if (character->GetFlags() & STATIC_GEOMETRY)
			continue;

		if (character->GetFlags() & DONT_INTEGRATE)
			continue;

		character->PostUpdate(delta);
	}

	for (auto character : sCharacters)
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

	sStaticStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	sDynamicStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	return 0;
}

uint32_t Simulator::GetId(const sf::Vector2i& position)
{
	Simulatable* sim = nullptr;

	sStaticStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	sDynamicStorage.IsPointColliding(position, sim);
	if (sim != nullptr)
		return sim->GetID();

	return 0;
}