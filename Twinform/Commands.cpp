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