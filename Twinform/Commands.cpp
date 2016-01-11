#include "Commands.h"
#include "Creator.h"

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