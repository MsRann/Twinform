#pragma once

#include <vector>
#include <string>

void OSGetFilesInDir(const std::string& dirName, std::vector<std::string>& files);
void OSDeleteFile(const std::string& dirName, const std::string& file);