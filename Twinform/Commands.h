#pragma once

#include <string>

class Command
{
public:
	virtual bool Execute() const = 0;
};

class SaveCommand : public Command
{
public:
	SaveCommand(const std::string& filename);

	bool Execute() const;
private:
	std::string mFilename;
};

class LoadCommand : public Command
{
public:
	LoadCommand(const std::string& filename);

	bool Execute() const;
private:
	std::string mFilename;
};

class ClearCommand : public Command
{
public:
	bool Execute() const;
};