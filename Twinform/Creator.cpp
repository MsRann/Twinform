#include "Creator.h"
#include "Simulator.h"
#include "Renderer.h"
#ifdef BUILDING
#include "Builder.h"
#endif

#include <unordered_map>
#include <stdint.h>
#include <iostream>
#include <fstream>

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "rapidjson\filereadstream.h"
#include "TwinMath.h"

// TODO: ID generation is bad as a number is simply incremented.
// This bug may never see the light of day but if a certain scene had a lot of particle effects
// that required a unique id and the client is run for a while it *could* eventually happen.

namespace
{
	// This storage can be used to look up characters by ID, maybe useful, maybe not.
	// Overhead is only a 32-bit uint as the key id
	std::unordered_map<uint32_t, ControllableCharacter> sControllableCharacters;
	std::unordered_map<uint32_t, StaticGeometry> sStaticGeometry;
	// This will just act as an auto increment integer
	uint32_t mUniqueId = 1;

	void DeleteStaticGeometry(const uint32_t& id);
	void DeleteControllableCharacter(const uint32_t& id);

	void DeleteStaticGeometry(const uint32_t& id)
	{
		if (Creator::GetStaticGeometry(id) == nullptr)
			return;

		StaticGeometry& geometry = sStaticGeometry[id];
		Renderer::Remove(id);
		Simulator::DeleteStatic(geometry);
		sStaticGeometry.erase(id);
	}

	void DeleteControllableCharacter(const uint32_t& id)
	{
		if (Creator::GetControllableCharacter(id) == nullptr)
			return;

		ControllableCharacter& character = sControllableCharacters[id];
		Renderer::Remove(id);
		Simulator::DeleteDynamic(character);
		sControllableCharacters.erase(id);

#ifdef BUILDING
		Builder::SetCharacterSpawned(false);
#endif
	}
}

void Creator::MakeStaticGeometry(const sf::Vector2f& position, const sf::Vector2f& size)
{
	if (mUniqueId == 0)
	{
		std::cout << "Overflow of 32 bit int" << std::endl;
		return;
	}

	sStaticGeometry[mUniqueId] = StaticGeometry(position, size, mUniqueId);
	Renderer::Add(mUniqueId, sStaticGeometry[mUniqueId].GetDrawable());
	Simulator::Add(sStaticGeometry[mUniqueId]);
	++mUniqueId;
}

void Creator::MakeStaticGeometryFromSize(const sf::Vector2i& position, const sf::Vector2f& size)
{
	if (mUniqueId == 0)
	{
		std::cout << "Overflow of 32 bit int" << std::endl;
		return;
	}

	sf::Vector2f pos;
	pos.x = static_cast<REAL>(position.x) * size.x;
	pos.y = static_cast<REAL>(position.y) * size.y;
	sStaticGeometry[mUniqueId] = StaticGeometry(pos, size, mUniqueId);
	Renderer::Add(mUniqueId, sStaticGeometry[mUniqueId].GetDrawable());
	Simulator::Add(sStaticGeometry[mUniqueId]);
	++mUniqueId;
}

void Creator::MakeStaticGeometry(const sf::Vector2i& position, const sf::Vector2f& size)
{
	sf::Vector2f positionf((float)position.x, (float)position.y);
	MakeStaticGeometry(positionf, size);
}

void Creator::MakeControllableCharacter(const sf::Vector2f& start, const sf::Vector2f& size, ControllableControls controls, const sf::Vector2f& gravity)
{
	if (mUniqueId == 0)
	{
		std::cout << "Overflow of 32 bit int" << std::endl;
		return;
	}

	sControllableCharacters[mUniqueId] = ControllableCharacter(start, size, controls, mUniqueId);
	sControllableCharacters[mUniqueId].SetGravity(gravity);
	Renderer::Add(mUniqueId, sControllableCharacters[mUniqueId].GetDrawable());
	Simulator::Add(sControllableCharacters[mUniqueId]);
	++mUniqueId;
}

ControllableCharacter* Creator::GetControllableCharacter(const uint32_t& id)
{
	if (sControllableCharacters.find(id) == sControllableCharacters.end())
		return nullptr;

	return &sControllableCharacters[id];
}

StaticGeometry* Creator::GetStaticGeometry(const uint32_t& id)
{
	if (sStaticGeometry.find(id) == sStaticGeometry.end())
		return nullptr;

	return &sStaticGeometry[id];
}

bool Creator::GetControllableCharacters(std::vector<ControllableCharacter*>& characters)
{
	if (sControllableCharacters.size() == 0)
		return false;

	characters.clear();
	for (auto &character : sControllableCharacters)
		characters.push_back(&character.second);

	return true;
}

void Creator::Save(const std::string& filename)
{
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value map;
	map.SetString(filename.data(), static_cast<rapidjson::SizeType>(filename.size()), allocator);
	doc.AddMember("map", map, allocator);

	rapidjson::Value geomArr(rapidjson::kArrayType);
	for (auto g : sStaticGeometry)
	{
		sf::Vector2f p = g.second.GetParticle().GetPosition();
		sf::Vector2i tile = twinmath::CreateGrid(p, GRID_WIDTH_HALF, GRID_HEIGHT_HALF);
		rapidjson::Value geom;
		geom.SetObject();
		geom.AddMember("x", tile.x, allocator);
		geom.AddMember("y", tile.y, allocator);

		geomArr.PushBack(geom, allocator);
	}

	doc.AddMember("static_geometry", geomArr, allocator);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string json(buffer.GetString(), buffer.GetSize());
	std::string file = "Maps/" + filename;
	std::ofstream of(file);
	of << json;
	of.close();
}

void Creator::Load(const std::string& filename)
{
	Creator::Clear();
	std::string fullname = "Maps/" + filename;
	FILE* file = fopen(fullname.c_str(), "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(file, buffer, sizeof(buffer));
	rapidjson::Document doc;
	doc.ParseStream(is);
	const rapidjson::Value& geom = doc["static_geometry"];
	for (rapidjson::SizeType i = 0; i < geom.Size(); i++)
	{
		const rapidjson::Value& v = geom[i];
		sf::Vector2i coord = sf::Vector2i(v["x"].GetInt(), v["y"].GetInt());
		MakeStaticGeometryFromSize(coord, sf::Vector2f(25.0f, 25.0f));
	}
}

void Creator::Delete(const uint32_t& id)
{
	TwinformObject type = GetType(id);

	switch (type)
	{
		case CONTROLLABLE:
			{
				DeleteControllableCharacter(id);
			}
			break;
		case STATIC:
			{
				DeleteStaticGeometry(id);
			}
			break;
		case UNDETERMINABLE:
			{
				std::cout << "Unable to delete object with id " << id << std::endl;
			}
			break;
		default:
			break;
	}
}

void Creator::Clear()
{
	std::unordered_map<uint32_t, StaticGeometry>::iterator itr = sStaticGeometry.begin();
	while (itr != sStaticGeometry.end())
	{
		uint32_t id = itr->second.GetID();
		std::unordered_map<uint32_t, StaticGeometry>::iterator toerase = itr;
		++itr;
		Creator::Delete(id);
	}

	std::unordered_map<uint32_t, ControllableCharacter>::iterator itr2 = sControllableCharacters.begin();
	while (itr2 != sControllableCharacters.end())
	{
		uint32_t id = itr2->second.GetID();
		std::unordered_map<uint32_t, ControllableCharacter>::iterator toerase = itr2;
		++itr2;
		Creator::Delete(id);
	}

	mUniqueId = 1;
}

TwinformObject Creator::GetType(const uint32_t& id)
{
	if (sControllableCharacters.find(id) != sControllableCharacters.end())
		return CONTROLLABLE;

	if (sStaticGeometry.find(id) != sStaticGeometry.end())
		return STATIC;

	return UNDETERMINABLE;
}