/*
* This file is a true hunk of crap.
* I need a way of storing all game objects by type and this is the poor module that suffers for it.
* In here ther will be lots of code duplication.
* To make matters worse this is where I save and load to/from file.
* TODO: Fix it.
*/

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
  std::unordered_map<uint32_t, Neutron*> sNeutrons;
  std::unordered_map<uint32_t, Proton*> sProtons;
  std::unordered_map<uint32_t, Electron*> sElectrons;
  std::unordered_map<uint32_t, StaticGeometry*> sStaticGeometry;
  std::unordered_map<uint32_t, Collectible*> sCollectibles;
  Neutron* sPlayerNeutron = nullptr;
  Electron* sPlayerElectron = nullptr;
  Proton* sPlayerProton = nullptr;
  // This will just act as an auto increment integer
  uint32_t mUniqueId = 1;

  void DeleteStaticGeometry(const uint32_t& id);
  void DeleteNeutron(const uint32_t& id);
  void DeleteProton(const uint32_t& id);
  void DeleteElectron(const uint32_t& id);
  void DeleteCollectible(const uint32_t& id);

  void DeleteStaticGeometry(const uint32_t& id)
  {
    if (sStaticGeometry.find(id) == sStaticGeometry.end())
      return;

    StaticGeometry* geometry = sStaticGeometry[id];
    Renderer::Remove(id);
    Simulator::DeleteStatic(*geometry);
    sStaticGeometry.erase(id);
    delete geometry;
  }

  void DeleteNeutron(const uint32_t& id)
  {
    if (sNeutrons.find(id) == sNeutrons.end())
      return;

    Neutron* neutron = sNeutrons[id];
    Renderer::Remove(id);
    Simulator::DeleteDynamic(*neutron);
    sNeutrons.erase(id);
    delete neutron;
  }

  void DeleteProton(const uint32_t& id)
  {
    if (sProtons.find(id) == sProtons.end())
      return;

    Proton* proton = sProtons[id];
    Renderer::Remove(id);
    Simulator::DeleteDynamic(*proton);
    sNeutrons.erase(id);
    delete proton;
  }

  void DeleteElectron(const uint32_t& id)
  {
    if (sElectrons.find(id) == sElectrons.end())
      return;

    Electron* electron = sElectrons[id];
    Renderer::Remove(id);
    Simulator::DeleteDynamic(*electron);
    sNeutrons.erase(id);
    delete electron;
  }

  void DeleteCollectible(const uint32_t& id)
  {
    if (sCollectibles.find(id) == sCollectibles.end())
      return;

    Collectible* collectible = sCollectibles[id];
    Renderer::Remove(id);
    Simulator::DeleteDynamic(*collectible);
    sCollectibles.erase(id);
    delete collectible;
  }
}

StaticGeometry* Creator::MakeStaticGeometry(
  const sf::Vector2f& position
  , const sf::Vector2f& size)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sStaticGeometry[mUniqueId] = new StaticGeometry(position, size, mUniqueId);
  Renderer::Add(mUniqueId, sStaticGeometry[mUniqueId]->GetDrawable());
  Simulator::Add(*sStaticGeometry[mUniqueId]);
  ++mUniqueId;
  return sStaticGeometry[mUniqueId - 1];
}

StaticGeometry* Creator::MakeStaticGeometryFromSize(
  const sf::Vector2i& position
  , const sf::Vector2f& size)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sf::Vector2f pos;
  pos.x = static_cast<REAL>(position.x) * size.x;
  pos.y = static_cast<REAL>(position.y) * size.y;
  sStaticGeometry[mUniqueId] = new StaticGeometry(pos, size, mUniqueId);
  Renderer::Add(mUniqueId, sStaticGeometry[mUniqueId]->GetDrawable());
  Simulator::Add(*sStaticGeometry[mUniqueId]);
  ++mUniqueId;
  return sStaticGeometry[mUniqueId - 1];
}

StaticGeometry* Creator::MakeStaticGeometry(
  const sf::Vector2i& position
  , const sf::Vector2f& size)
{
  sf::Vector2f positionf((float)position.x, (float)position.y);
  return MakeStaticGeometry(positionf, size);
}

Neutron* Creator::MakeNeutron(
  Brain* brain
  , const sf::Vector2f& start
  , const sf::Vector2f& size
  , const sf::Vector2f& gravity)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sNeutrons[mUniqueId] = new Neutron(
    brain
    , start
    , size
    , mUniqueId);

  sNeutrons[mUniqueId]->SetGravity(gravity);
  Renderer::Add(mUniqueId, sNeutrons[mUniqueId]->GetDrawable());
  Simulator::Add(*sNeutrons[mUniqueId]);
  if (!sPlayerNeutron)
  {
    sPlayerNeutron = sNeutrons[mUniqueId];
  }
  ++mUniqueId;

  return sNeutrons[mUniqueId - 1];
}

Electron* Creator::MakeElectron(
  Brain* brain
  , const sf::Vector2f& start
  , const sf::Vector2f& size
  , const sf::Vector2f& gravity)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sElectrons[mUniqueId] = new Electron(
    brain
    , start
    , size
    , mUniqueId);

  sElectrons[mUniqueId]->SetGravity(gravity);
  Renderer::Add(mUniqueId, sElectrons[mUniqueId]->GetDrawable());
  Simulator::Add(*sElectrons[mUniqueId]);
  if (!sPlayerElectron)
  {
    sPlayerElectron = sElectrons[mUniqueId];
  }
  ++mUniqueId;

  return sElectrons[mUniqueId - 1];
}

Proton* Creator::MakeProton(
  Brain* brain
  , const sf::Vector2f& start
  , const sf::Vector2f& size
  , const sf::Vector2f& gravity)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sProtons[mUniqueId] = new Proton(
    brain
    , start
    , size
    , mUniqueId);

  sProtons[mUniqueId]->SetGravity(gravity);
  Renderer::Add(mUniqueId, sProtons[mUniqueId]->GetDrawable());
  Simulator::Add(*sProtons[mUniqueId]);
  if (!sPlayerProton)
  {
    sPlayerProton = sProtons[mUniqueId];
  }
  ++mUniqueId;

  return sProtons[mUniqueId - 1];
}

Collectible* Creator::MakeCollectible(
  const sf::Vector2f& start
  , const sf::Vector2f& size)
{
  if (mUniqueId == 0)
  {
    return nullptr;
  }

  sCollectibles[mUniqueId] = new Collectible(start, size, mUniqueId);
  Renderer::Add(mUniqueId, sCollectibles[mUniqueId]->GetDrawable());
  Simulator::Add(*sCollectibles[mUniqueId]);
  ++mUniqueId;
  return sCollectibles[mUniqueId - 1];
}

Neutron* Creator::GetPlayerNeutron()
{
  return sPlayerNeutron;
}

Proton* Creator::GetPlayerProton()
{
  return sPlayerProton;
}

Electron* Creator::GetPlayerElectron()
{
  return sPlayerElectron;
}

void Creator::SetPlayerNeutron(Neutron* neutron)
{
  sPlayerNeutron = neutron;
}

void Creator::SetPlayerProton(Proton* proton)
{
  sPlayerProton = proton;
}

void Creator::SetPlayerElectron(Electron* electron)
{
  sPlayerElectron = electron;
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
    sf::Vector2f p = g.second->GetParticle().GetPosition();
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
    case NEUTRON:
      {
        DeleteNeutron(id);
      }
      break;
    case ELECTRON:
      {
        DeleteElectron(id);
      }
      break;
    case PROTON:
      {
        DeleteProton(id);
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
  // Clear all the maps of their stuff
  std::unordered_map<uint32_t, StaticGeometry*>::iterator itr = sStaticGeometry.begin();
  while (itr != sStaticGeometry.end())
  {
    uint32_t id = itr->second->GetID();
    std::unordered_map<uint32_t, StaticGeometry*>::iterator toerase = itr;
    ++itr;
    Creator::Delete(id);
  }

  std::unordered_map<uint32_t, Neutron*>::iterator itr2 = sNeutrons.begin();
  while (itr2 != sNeutrons.end())
  {
    uint32_t id = itr2->second->GetID();
    std::unordered_map<uint32_t, Neutron*>::iterator toerase = itr2;
    ++itr2;
    Creator::Delete(id);
  }

  std::unordered_map<uint32_t, Collectible*>::iterator itr3 = sCollectibles.begin();
  while (itr3 != sCollectibles.end())
  {
    uint32_t id = itr2->second->GetID();
    std::unordered_map<uint32_t, Collectible*>::iterator toerase = itr3;
    ++itr3;
    Creator::Delete(id);
  }

  std::unordered_map<uint32_t, Proton*>::iterator itr4 = sProtons.begin();
  while (itr4 != sProtons.end())
  {
    uint32_t id = itr4->second->GetID();
    std::unordered_map<uint32_t, Proton*>::iterator toerase = itr4;
    ++itr4;
    Creator::Delete(id);
  }

  std::unordered_map<uint32_t, Electron*>::iterator itr5 = sElectrons.begin();
  while (itr5 != sElectrons.end())
  {
    uint32_t id = itr5->second->GetID();
    std::unordered_map<uint32_t, Electron*>::iterator toerase = itr5;
    ++itr2;
    Creator::Delete(id);
  }

  mUniqueId = 1;
}

TwinformObject Creator::GetType(const uint32_t& id)
{
  // Is this dumb?

  // Yeah,
  //  This is stupid.

  // This needs to be some base class, 'Creatable' perhaps

  if (sNeutrons.find(id) != sNeutrons.end())
    return NEUTRON;

  if (sProtons.find(id) != sProtons.end())
    return PROTON;

  if (sElectrons.find(id) != sElectrons.end())
    return ELECTRON;

  if (sStaticGeometry.find(id) != sStaticGeometry.end())
    return STATIC;

  if (sCollectibles.find(id) != sCollectibles.end())
    return COLLECTIBLE;

  return UNDETERMINABLE;
}