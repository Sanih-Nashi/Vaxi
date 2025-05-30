#include <filesystem>
#include <cstring>

#include "definitions.h"

void ListDirContents()
{
  
  for (const auto& entry : std::filesystem::directory_iterator(CWD))
    DirContents.push_back(std::filesystem::relative(entry.path(), CWD).string());

}

char* LastWord(char* str)
{
  char* Last = str;
  for (int i = 0; str[i] != '\0'; i++)
  {
    if ((str[i] == ' ' || str[i] == '/' ) && str[i + 1] != '\0' && str[i + 1] != ' ')
      Last = &str[i + 1];
  }
  
  return Last;

}
