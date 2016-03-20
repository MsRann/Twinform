#include "PropertyReader.h"

#include <fstream>
#include <iostream>

PropertyReader::PropertyReader()
{

}

PropertyReader::PropertyReader(const std::string& FileName)
{
  LoadFile(FileName);
}

PropertyReader::~PropertyReader()
{

}

void PropertyReader::LoadFile(const std::string& FileName)
{
  std::ifstream File(FileName);
  if (!File.is_open())
  {
    std::cerr << "Unable to open properties file: " << FileName << std::endl;
    std::exit(1);
  }

  std::string Line;
  while (std::getline(File, Line))
  {
    // If first character on line is # assume comment, if it's blank assume malformed/incorrect/empty line and skip it
    if (Line[0] == '#' || Line[0] == ' ' || Line.empty())
      continue;

    std::string Key;
    std::string Value;
    unsigned int Index = 0;
    size_t EqualIndex = Line.find_first_of('=');
    // Extract key
    for (Index; Index < EqualIndex; ++Index)
    {
      if (Line[Index] != ' ')
        Key += Line[Index];
    }

    // Extract value
    for (Index = (unsigned int)EqualIndex + 1; Index < Line.size(); ++Index)
    {
      if (Line[Index] != ' ')
        Value += Line[Index];
    }

    mProperties[Key] = Value;
  }
  File.close();
}

void PropertyReader::Free()
{
  mProperties.clear();
}

bool PropertyReader::ReadInt(const std::string& Key, int& Value)
{
  if (mProperties.empty())
  {
    std::cerr << "No properties file loaded" << std::endl;
    return false;
  }

  ConstMapIterator It = mProperties.find(Key);
  if (It != mProperties.end())
  {
    Value = std::stoi(It->second);
    return true;
  }

  return false;
}

bool PropertyReader::ReadFloat(const std::string& Key, float& Value)
{
  if (mProperties.empty())
  {
    std::cerr << "No properties file loaded" << std::endl;
    return false;
  }

  ConstMapIterator It = mProperties.find(Key);
  if (It != mProperties.end())
  {
    Value = std::stof(It->second);
    return true;
  }

  return false;
}

bool PropertyReader::ReadString(const std::string& Key, std::string& Value)
{
  if (mProperties.empty())
  {
    std::cerr << "No properties file loaded" << std::endl;
    return false;
  }

  ConstMapIterator It = mProperties.find(Key);
  if (It != mProperties.end())
  {
    Value = It->second;
    return true;
  }

  return false;
}