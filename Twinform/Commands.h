#pragma once

#include <string>
#include <cstdint>

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

class PickupCommand : public Command
{
public:
  PickupCommand(uint32_t itemid);

  bool Execute() const;
private:
  uint32_t mItemId;
};