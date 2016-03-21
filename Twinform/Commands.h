#pragma once

#include "Simulatable.h"

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

  virtual bool Execute() const override;
private:
  std::string mFilename;
};

class LoadCommand : public Command
{
public:
  LoadCommand(const std::string& filename);

  virtual bool Execute() const override;
private:
  std::string mFilename;
};

class ClearCommand : public Command
{
public:
  virtual bool Execute() const override;
};

class PickupCommand : public Command
{
public:
  PickupCommand(uint32_t itemid);

  virtual bool Execute() const override;
private:
  uint32_t mItemId;
};

class SplitNeutronCommand : public Command
{
public:
  SplitNeutronCommand(Simulatable* sim);

  virtual bool Execute() const override;
private:
  Simulatable* mSimulatable;
};