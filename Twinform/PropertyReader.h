#pragma once
#include <string>
#include <unordered_map>

class PropertyReader
{
public:
	typedef std::unordered_map<std::string, std::string>::const_iterator ConstMapIterator;
	typedef std::unordered_map<std::string, std::string> PropertiesMap;

	PropertyReader();
	PropertyReader(const std::string& FileName);
	~PropertyReader();

	void LoadFile(const std::string& FileName);
	// This is just to clear items in map if you are keeping PropertyReader in scope but want memory back
	// Else the destructor will handle clearing memory
	void Free();

	// Return false if no properties loaded or value does not exist in file
	bool ReadInt(const std::string& Key, int& Value);
	bool ReadFloat(const std::string& Key, float& Value);
	bool ReadString(const std::string& Key, std::string& Value);

private:
	PropertiesMap mProperties;
};