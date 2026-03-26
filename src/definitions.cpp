#include <filesystem>
#include <cstring>

#include "definitions.h"

void ListDirContents(std::vector<std::string>& DirFile, const std::string& WorkingDir)
{
  
  for (const auto& entry : std::filesystem::directory_iterator(WorkingDir))
    DirFile.push_back(std::filesystem::relative(entry.path(), WorkingDir).string());

}

bool LastWordAndRelativePath(char* str, char** LastWord, char** RelativeDirPath)
{


  *LastWord = &str[0];
  int LastWordIndex = 0;
  int DirStartIndex = 0;

  
  for (int i = 0; str[i] != '\0'; i++)
  {

  
    if (str[i] == ' ' && str[i + 1] != '\0' && str[i + 1] != ' ')
    {
    
      DirStartIndex = i + 1;
      LastWordIndex = i + 1;
      *LastWord = &str[LastWordIndex];
      
	}

	
	else if (str[i] == '/' && str[i + 1] != '\0' && str[i + 1] != ' ')
	{
		
	  LastWordIndex = i + 1;
	  *LastWord = &str[i + 1];
	  
	}


	else if (str[i] == '"')
	{
	
		while (str[i] != '"' && str[i] != '\0')
		  i++;

		if (str[i] != '"')
		  return false;

	}

	
  }

  // plus one because the LastWord will always point to the first letter of the word
  // but RelativePath will always point to the first character in the path which necessarily
  // does not need to be a letter like it can be "/"
  if (LastWordIndex + 1 == DirStartIndex || LastWordIndex == DirStartIndex) 
  {
	*RelativeDirPath = nullptr;
	return true;
  }

  const int Len = LastWordIndex - DirStartIndex;

  *RelativeDirPath = (char*)malloc(Len + 1); // plus 1 for the null terminator
  strncpy(*RelativeDirPath, &str[DirStartIndex], Len);
  (*RelativeDirPath)[Len] = '\0';
  
  return true;


  // Im so bad at writing code. I am reviewing it a year after I left it untouched
  // Imma change all of ts

}





