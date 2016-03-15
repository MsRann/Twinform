#pragma once

#include <vector>

#include "TwinformTypes.h"
#include "ControllableCharacter.h"
#include "Collectible.h"
#include "StaticGeometry.h"

namespace Creator
{
	void MakeStaticGeometry(const sf::Vector2f& position, const sf::Vector2f& size);
	void MakeStaticGeometry(const sf::Vector2i& position, const sf::Vector2f& size);
	void MakeStaticGeometryFromSize(const sf::Vector2i& position, const sf::Vector2f& size);
	void MakeControllableCharacter(const sf::Vector2f& start, const sf::Vector2f& size, ControllableControls controls, const sf::Vector2f& gravity);
	void MakeCollectible(const sf::Vector2f& start, const sf::Vector2f& size);

	ControllableCharacter* GetControllableCharacter(const uint32_t& id);
	StaticGeometry* GetStaticGeometry(const uint32_t& id);
	Collectible* GetCollectible(const uint32_t& id);

	bool GetControllableCharacters(std::vector<ControllableCharacter*>& characters);

	void Save(const std::string& filename);
	void Load(const std::string& filename);

	// Will delete the object with some id
	void Delete(const uint32_t& id);
	void Clear();
	TwinformObject GetType(const uint32_t& id);
}