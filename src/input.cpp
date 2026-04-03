#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <filesystem>


#include "input.h"
#include "init.h"
#include "definitions.h"


// TODO: * make it recieve multilined input
//       * make the autocorrect work with multilined corrections



char ReadKey(){
  int read_num;
  char c;
  while ( (read_num = read(STDIN_FILENO, &c, 1)) != 1) 
  {
    if (read_num == -1 && errno != EAGAIN)
    {
      std::perror("read");
      exit(1);
    }
  }
  return c;
}


int ReadInput(std::string& Input)
{
  char c;
  std::string InspectingDir = CWD; 
  // this variable is used for storing the path of directory currently inspecting
  // I am bad at explaining so...
  // Ex: if you click tab after typing ur_directory/Vx-Cross/bui
  // it will auto complete the last word to build but the folder build is not in the cwd so it stores the path
  // to that in the inspecting folder

  while (true)
  {

    c = ReadKey();
    
    // visible characters
    if (32 <= c && c <= 126)
    {

      std::cout <<c <<std::flush;      
      Input.push_back(c);

      continue;
    }



    switch (c)
    {

      case ENTER_KEY:
      {
      
		std::cout << "\n\r" <<std::flush;
        Input.push_back('\0');  // giving it an endline charactor immediately after the command in the array
        return EXIT_SUCCESS;
        
      }

      case BACKSPACE:
      case DEL_KEY:
      {
        if (Input.size() > 0)
        {
        
		  std::cout <<"\b \b" <<std::flush;
          Input.pop_back();
          
        }
        break;
      }



      case TAB:
      {

      
      	if (Input.size() == 0)
      	  continue;
      	
        // TODO: Change the c-style pointer upproach to more c++ way
        std::string LastWord;
        std::string RelativePathDir;
        int LastWordLen;


		// last word here is the last word of the command Ex:- cd Vx-Cross/bui, here it is "bui"
		// relative path will get "Vx-Cross/" out of the above command
        if (!LastWordAndRelativePath(Input, LastWord, RelativePathDir))
          continue;

        LastWordLen = LastWord.size();

        if (RelativePathDir != "")
          InspectingDir = CWD + "/" + RelativePathDir;
	

        // matching the file user typed incompletely and wanted it to autocompleted to the directory.
		std::vector<std::string> FileMatch;
		for (const auto& DirContent : std::filesystem::directory_iterator{std::filesystem::path(InspectingDir)})
		{

		  std::string MatchedFile = DirContent.path().filename().string();
	      if (MatchedFile.substr(0, LastWordLen) == LastWord)
	      {
	      
            if (std::filesystem::is_directory(std::filesystem::path(MatchedFile)))
              FileMatch.push_back(MatchedFile + "/");
              
            else
	          FileMatch.push_back(MatchedFile); 
	                       
		  } 	
		}

		if (FileMatch.size() == 1)
		{
		  // autocompleting the last word
	      std::string RemainingStr = FileMatch[0].substr(LastWordLen);
		  std::cout << RemainingStr <<std::flush;
		  
		  Input += RemainingStr;
			
		}
		else if (FileMatch.size() == 0)
		{
		  //TODO:- check the /bin and /usr for the described word
		}
		//more than one
		else
		{

		
		  // trying to type in the remaining common string if it has any
		  // Ex:- if there are two file like input.h and input.cpp and you click tab on "in"
		  //      then it will complete upto "input."
		  
		  bool IsCharSame = true;
		  for (int i = LastWordLen; IsCharSame; i++)
		  {
		  	char Letter = FileMatch[0][i];
		  	// checks whether the letter is in every matched file
		  	for (const std::string& File : FileMatch)
		  	  {
		  	    // if it doesn't, it will print the string upto there, add it to the Input
		  	    // and breaks out of the loop
		  	  	if (File[i] != Letter)
		  	  	{
		  	  	  IsCharSame = false;
		  	  	  std::string RemainingCommonStr = File.substr(LastWordLen, i - LastWordLen);
		  	  	  Input += RemainingCommonStr;
		  	  	  std::cout <<RemainingCommonStr <<std::flush;
		  	  	  break;
		  	  	}
		  	  }
		  	
		  }

		  // prints the matched files in the next line, and then prints the prompt and the
		  // written command again for the user to complete the command
		  std::cout <<"\n\r" <<std::flush;

		  for (const std::string& MatchedFile : FileMatch)
		    std::cout <<"  " <<MatchedFile <<std::flush;
		  std::cout <<"\n\r" <<std::flush;

		  PrintPrompt();
		  std::cout <<Input <<std::flush;

		  
		}

		 		
		
		
        
        break;
      }

        


    };

  }
}
