#include "OSUtils.h"

#include <cstdint>

// Dirent on windows is an abstraction to windows API calls. 
#include "include\dirent.h"

namespace
{
  static const uint32_t sMaxFileLength = 256;
}

void OSGetFilesInDir(const std::string& dirName, std::vector<std::string>& files)
{
  DIR* dir;
  struct dirent* ent;
  if ((dir = opendir(dirName.c_str())) != NULL) 
  {
    // print all the files and directories within directory
    while ((ent = readdir(dir)) != NULL) 
    {
      files.push_back(ent->d_name);
    }
    closedir(dir);
  }
  else 
  {
    // could not open directory
    perror("Could not open directory");
  }
}

void OSDeleteFile(const std::string& dirName, const std::string& file)
{
  DIR* dir = opendir(dirName.c_str());
  if (!dir)
  {
    perror("Could not open directory");
    return;
  }
  struct dirent* ent;
  while ((ent = readdir(dir)) != NULL)
  {
    if (file == ent->d_name)
    {
      // Max length of filepath
      char filepath[sMaxFileLength];
      memset(filepath, 0, sMaxFileLength);
      strcat(filepath, dirName.c_str());
      strcat(filepath, "/");
      strcat(filepath, ent->d_name);
      remove(filepath);
      printf("%s deleted\n", filepath);
    }
  }
  closedir(dir);
}