#include "HashedCellStorage.h"

#include <iostream>

HashedCellStorage::HashedCellStorage()
{
}


HashedCellStorage::~HashedCellStorage()
{
}

void HashedCellStorage::Insert(Simulatable& simulatable)
{
	sf::Vector2i key = CreateKey(simulatable);
	simulatable.SetPrevKey(key);
	InsertToKey(simulatable, key);
}

void HashedCellStorage::InsertToKey(Simulatable& simulatable, sf::Vector2i key)
{
	mGrid[key].push_back(&simulatable);
}

void HashedCellStorage::Remove(Simulatable& simulatable)
{
	// Clamp to a cell
	sf::Vector2i key = simulatable.GetPrevKey();
	// Find list by grid cell the collidable object is in
	CollisionMap::const_iterator got = mGrid.find(key);
	if (got != mGrid.end())
	{
		for (std::vector<Simulatable*>::iterator it = mGrid[key].begin(); it != mGrid[key].end(); ++it)
		{
			if (*it == &simulatable)
			{
				mGrid[key].erase(it);
				break;
			}
		}
		if (mGrid[key].size() == 0)
		{
			mGrid.erase(key);
		}
	}
}

bool HashedCellStorage::OnGround(Simulatable& simulatable)
{
	Particle particleCopy = simulatable.GetParticle();
	// This is only possible since we only move in fixed delta times
	// Integrate the particle forward one step, with only gravity, if it is hitting something it's gravity
	// is causing it to come in contact with the ground and is therefore on the ground
	particleCopy.SetVelocity(0.0f, 0.0f);
	particleCopy.SetAcceleration(0.0f, 0.0f);
	particleCopy.SetAcceleration(simulatable.GetGravity());
	particleCopy.Integrate(MAX_DT);
	sf::Vector2f pos = particleCopy.GetPosition();
	sf::FloatRect bounds(pos.x, pos.y, simulatable.GetCollisionBounds().width, simulatable.GetCollisionBounds().height);
	sf::Vector2i key = CreateKey(particleCopy.GetPosition());

	if (mGrid.find(key) == mGrid.end())
		return false;

	for (auto sim : mGrid[key])
	{
		if (sim->GetCollisionBounds().intersects(bounds))
			return true;
	}

	return false;
}

void HashedCellStorage::UpdateCells()
{
	std::vector<Simulatable*> toUpdate;

	for (auto bucket : mGrid)
	{
		for (auto sim : bucket.second)
		{
			// Is this bad? Should it error or continue? I have no idea.
			if (sim == nullptr)
			{
				std::cout << "HASHED GRID CELL HAS NULLPTR!!!!!" << std::endl;
				continue;
			}

			sf::Vector2i prevKey = sim->GetPrevKey();
			// Calculate new key
			sf::Vector2i key = CreateKey(*sim);
			// If they differ update the map
			if (prevKey != key)
				toUpdate.push_back(sim);
		}
	}

	for (auto update : toUpdate)
	{
		Remove(*update);
		Insert(*update);
	}
}

bool HashedCellStorage::IsColliding(Simulatable& simulatable)
{
	std::vector<std::pair<Simulatable*, sf::FloatRect>> outs;
	return IsColliding(simulatable, outs);
}

bool HashedCellStorage::IsColliding(Simulatable& simulatable, std::vector<std::pair<Simulatable*, sf::FloatRect>>& simulatablesHit)
{
	sf::Vector2i key = simulatable.GetPrevKey();

	// Get all adjacent keys, right, left, up, down
	// TODO: If the simulatable will surpass a grid in a single update this won't work (that situation is also unlikely)
	std::vector<sf::Vector2i> keys;
	keys.push_back(key);
	keys.push_back(key + sf::Vector2i(1, 0));
	keys.push_back(key + sf::Vector2i(-1, 0));
	keys.push_back(key + sf::Vector2i(0, -1));
	keys.push_back(key + sf::Vector2i(0, 1));
	// Get diagonal keys
	keys.push_back(key + sf::Vector2i(1, 1));
	keys.push_back(key + sf::Vector2i(-1, 1));
	keys.push_back(key + sf::Vector2i(1, -1));
	keys.push_back(key + sf::Vector2i(-1, -1));

	for (auto key : keys)
	{
		if (mGrid.find(key) == mGrid.end())
			continue;

		for (auto sim : mGrid[key])
		{
			// Can't collide with itself
			if (sim != &simulatable)
			{
				// Intersection is the square of the intersection with the simulatable
				sf::FloatRect intersection;
				if (simulatable.GetCollisionBounds().intersects(sim->GetCollisionBounds(), intersection))
					simulatablesHit.push_back(std::pair<Simulatable*, sf::FloatRect>(sim, intersection));
			}
		}
	}

	if (simulatablesHit.size() > 0)
		return true;

	return false;
}

bool HashedCellStorage::IsPointColliding(sf::Vector2f point)
{
	Simulatable* sim = nullptr;

	return IsPointColliding(point, sim);
}

bool HashedCellStorage::IsPointColliding(sf::Vector2i point)
{
	Simulatable* sim = nullptr;

	return IsPointColliding(point, sim);
}

bool HashedCellStorage::IsPointColliding(sf::Vector2f point, Simulatable*& simulatableHit)
{
	sf::Vector2i key = CreateKey(point);

	for (auto sim : mGrid[key])
	{
		if (sim->GetCollisionBounds().contains(point))
		{
			simulatableHit = sim;
			return true;
		}
	}

	return false;
}

bool HashedCellStorage::IsPointColliding(sf::Vector2i point, Simulatable*& simulatableHit)
{
	sf::Vector2i key = CreateKey(point);
	sf::Vector2f pointf((float)point.x, (float)point.y);

	for (auto sim : mGrid[key])
	{
		if (sim->GetCollisionBounds().contains(pointf))
		{
			simulatableHit = sim;
			return true;
		}
	}

	return false;
}

sf::Vector2i HashedCellStorage::CreateKey(const Simulatable& sim) const
{
	sf::Vector2f pos = sim.GetParticleConst().GetPosition();
	return CreateKey(pos);
}

sf::Vector2i HashedCellStorage::CreateKey(const sf::Vector2f& pos) const
{
	return sf::Vector2i((int)floor(pos.x / GRID_WIDTH), (int)floor(pos.y / GRID_HEIGHT));
}

sf::Vector2i HashedCellStorage::CreateKey(const sf::Vector2i& pos) const
{
	return sf::Vector2i(pos.x / GRID_WIDTH, pos.y / GRID_HEIGHT);
}