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
  std::unordered_map<uint32_t, Collectible> sCollectibles;
  ControllableCharacter* sPlayer = nullptr;
  // This will just act as an auto increment integer
  uint32_t mUniqueId = 1;

  void DeleteStaticGeometry(const uint32_t& id);
  void DeleteControllableCharacter(const uint32_t& id);

  void DeleteStaticGeometry(const uint32_t& id)
  {
    if (sStaticGeometry.find(id) == sStaticGeometry.end())
      return;

    StaticGeometry& geometry = sStaticGeometry[id];
    Renderer::Remove(id);
    Simulator::DeleteStatic(geometry);
    sStaticGeometry.erase(id);
  }

  void DeleteControllableCharacter(const uint32_t& id)
  {
    if (sControllableCharacters.find(id) == sControllableCharacters.end())
      return;

    ControllableCharacter& character = sControllableCharacters[id];
    Renderer::Remove(id);
    Simulator::DeleteDynamic(character);
    sControllableCharacters.erase(id);

#ifdef BUILDING
    Builder::SetCharacterSpawned(false);
#endif
  }

  void DeleteCollectible(const uint32_t& id)
  {
    if (sCollectibles.find(id) == sCollectibles.end())
      return;

    Collectible& collectible = sCollectibles[id];
    Renderer::Remove(id);
    Simulator::DeleteDynamic(collectible);
    sCollectibles.erase(id);
  }
}

StaticGeometry* Creator::MakeStaticGeometry(const sf::Vector2f& position, const sf::Vector2f& size)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sStaticGeometry[mUniqueId] = StaticGeometry(position, size, mUniqueId);
  Renderer::Add(mUniqueId, sStaticGeometry[mUniqueId].GetDrawable());
  Simulator::Add(sStaticGeometry[mUniqueId]);
  ++mUniqueId;
  return &sStaticGeometry[mUniqueId - 1];
}

StaticGeometry* Creator::MakeStaticGeometryFromSize(const sf::Vector2i& position, const sf::Vector2f& size)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sf::Vector2f pos;
  pos.x = static_cast<REAL>(position.x) * size.x;
  pos.y = static_cast<REAL>(position.y) * size.y;
  sStaticGeometry[mUniqueId] = StaticGeometry(pos, size, mUniqueId);
  Renderer::Add(mUniqueId, sStaticGeometry[mUniqueId].GetDrawable());
  Simulator::Add(sStaticGeometry[mUniqueId]);
  ++mUniqueId;
  return &sStaticGeometry[mUniqueId - 1];
}

StaticGeometry* Creator::MakeStaticGeometry(const sf::Vector2i& position, const sf::Vector2f& size)
{
  sf::Vector2f positionf((float)position.x, (float)position.y);
  return MakeStaticGeometry(positionf, size);
}

ControllableCharacter* Creator::MakeControllableCharacter(const sf::Vector2f& start, const sf::Vector2f& size, ControllableControls controls, const sf::Vector2f& gravity)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sControllableCharacters[mUniqueId] = ControllableCharacter(start, size, controls, mUniqueId);
  sControllableCharacters[mUniqueId].SetGravity(gravity);
  Renderer::Add(mUniqueId, sControllableCharacters[mUniqueId].GetDrawable());
  Simulator::Add(sControllableCharacters[mUniqueId]);
  sPlayer = &sControllableCharacters[mUniqueId];
  ++mUniqueId;
  return &sControllableCharacters[mUniqueId - 1];
}

Collectible* Creator::MakeCollectible(const sf::Vector2f& start, const sf::Vector2f& size)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sCollectibles[mUniqueId] = Collectible(start, size, mUniqueId);
  Renderer::Add(mUniqueId, sCollectibles[mUniqueId].GetDrawable());
  Simulator::Add(sCollectibles[mUniqueId]);
  ++mUniqueId;
  return &sCollectibles[mUniqueId - 1];
}

ControllableCharacter* Creator::GetPlayer()
{
  return sPlayer;
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
    case COLLECTIBLE:
      {
        DeleteCollectible(id);
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
  // Is this dumb?

  // TODO:
  //  This is stupid.

  // This needs to be some base class, 'Creatable' perhaps

  if (sControllableCharacters.find(id) != sControllableCharacters.end())
    return CONTROLLABLE;

  if (sStaticGeometry.find(id) != sStaticGeometry.end())
    return STATIC;

  if (sCollectibles.find(id) != sCollectibles.end())
    return COLLECTIBLE;

  return UNDETERMINABLE;
}