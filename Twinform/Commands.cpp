#include "Commands.h"

#include "Creator.h"
#include "Inventory.h"

#include <iostream>

SaveCommand::SaveCommand(const std::string& filename) :
  mFilename(filename)
{
}

bool SaveCommand::Execute() const
{
  Creator::Save(mFilename);
  return true;
}

LoadCommand::LoadCommand(const std::string& filename) :
  mFilename(filename)
{
}

bool LoadCommand::Execute() const
{
  Creator::Load(mFilename);
  return true;
}

bool ClearCommand::Execute() const
{
  Creator::Clear();
  return true;
}

PickupCommand::PickupCommand(uint32_t id) :
  mItemId(id)
{
}

bool PickupCommand::Execute() const
{
  // TODO: This will need to do more
  Inventory::AddCollectible(); // Increment collectible count
  Creator::Delete(mItemId);
  return true;
}

SplitNeutronCommand::SplitNeutronCommand(Simulatable* sim) :
  mSimulatable(sim)
{
}

bool SplitNeutronCommand::Execute() const
{
  // Create the proton and electron
  Proton* proton = Creator::MakeProton(
    new PlayerControls(CONTROLS_WASD)
    , mSimulatable->GetParticle().GetPosition()
    , sf::Vector2f(25.0f, 25.0f)
    , sf::Vector2f(0.0f, 10.0f));

  Electron* electron = Creator::MakeElectron(
    new PlayerControls(CONTROLS_ARROWS)
    , mSimulatable->GetParticle().GetPosition()
    , sf::Vector2f(25.0f, 25.0f)
    , sf::Vector2f(0.0f, 10.0f));

  // Give them a force in the opposite direction
  proton->GetParticle().AddForce(
    sf::Vector2f(-150.0f, -150.0f));

  electron->GetParticle().AddForce(
    sf::Vector2f(150.0f, -150.0f));

  // Delete the neutron
  Creator::Delete(mSimulatable->GetID());

  return false;
}
