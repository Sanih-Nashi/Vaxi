#include <filesystem>
#include <cstring>
#include <string>

#include "definitions.h"


bool LastWordAndRelativePath(const std::string& Input, std::string& LastWord, std::string& RelativeDirPath)
{


  // these variables store the index from which LastWord and
  // and RelativePathDir start from the major string Input
  int LastWordIndex = 0;
  int DirStartIndex = 0;

  
  for (int i = 0; Input[i] != '\0'; i++)
  {

  
    if (Input[i] == ' ')
    {
    
      if (Input[i + 1] == '\0' && Input[i + 1] == ' ')
        continue;

      // setting the lastword and the starting of the dir to the char after lastword
      DirStartIndex = i + 1;
      LastWordIndex = i + 1;
      
	}

	// LastWord is set from the character after '/'
	// while RelativePathDir is set from the '/' itself 
	else if (Input[i] == '/' && Input[i + 1] != '\0' && Input[i + 1] != ' ')
	  LastWordIndex = i + 1;


    // goes through the entire string as the part inbetween in
    // considered the part of the current token
	else if (Input[i] == '"')
	{
	
	  while (Input[i] != '"' && Input[i] != '\0')
   	    i++;

	  if (Input[i] != '"')
	    return false;

	}

    // same as the latter
	else if (Input[i] == '\'')
	{
	
	  while (Input[i] != '\'' && Input[i] != '\0')
	    i++;

	  if (Input[i] != '\'')
	    return false;

	}


	
  }
  LastWord = Input.substr(LastWordIndex);

  // plus one because the LastWord will always point to the first letter of the word
  // but RelativePath will always point to the first character in the path which necessarily
  // does not need to be a letter like it can be "/"
  if (LastWordIndex + 1 == DirStartIndex || LastWordIndex == DirStartIndex) 
  {
	RelativeDirPath = "";
	return true;
  }

  RelativeDirPath = Input.substr(DirStartIndex);
  
  return true;

}





