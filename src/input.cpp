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




char ReadKey(){
  int read_num;
  char c;
  while ( (read_num = read(STDIN_FILENO, &c, 1)) != 1) 
  {
    if (read_num == -1 && errno != EAGAIN)
    {
      perror("read");
      exit(1);
    }
  }
  return c;
}


int ReadInput(char* Input, int Input_MaxLength)
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


	// TODO: make it to take any amount inputs (sorry I made a bad base when starting and I'm lazy lol)

      if (NoOfCharTyped >= Input_MaxLength) // checks whether the input excedes the limit of the array
        continue;
            
      printf("%c", c);
      fflush(stdout);
      NoOfCharTyped++;
      Input[NoOfCharTyped - 1] = c;

      continue;
    }



    switch (c)
    {

      case ENTER_KEY:
      {
        printf("\n\r");
        fflush(stdout);

        if (NoOfCharTyped >= Input_MaxLength)
        {
          NoOfCharTyped = 0;
          fprintf(stderr, "Vx: The command typed exceded the max characters of 1024\n\r    Try again with fewer lines\n\r");
          return EXIT_FAILURE;
        }

        Input[NoOfCharTyped] = '\0';  // giving it an endline charactor immediately after the command in the array
        NoOfCharTyped = 0;
        return EXIT_SUCCESS;
      }

      case BACKSPACE:
      case DEL_KEY:
      {
        if (NoOfCharTyped > 0)
        {

          printf("\b \b");
          fflush(stdout);
          NoOfCharTyped--;
        }
        break;
      }



      case TAB:
      {

      
      	if (NoOfCharTyped ==0)
      	  continue;
      	
        Input[NoOfCharTyped] = '\0'; // making the last char null termination

        char* LastWord;
        char* RelativePathDir;
        int LastWordLen;


		// last word here is the last word of the command Ex:- cd Vx-Cross/bui, here it is "bui"
		// relative path will get "Vx-Cross/" out of the above command
        if (!LastWordAndRelativePath(Input, &LastWord, &RelativePathDir))
          continue;

        LastWordLen = strlen(LastWord);

        if (RelativePathDir != nullptr)
        {
          InspectingDir = std::string(CWD) + "/" + std::string(RelativePathDir);
          free(RelativePathDir);
        }
	

		std::vector<std::string> FileMatch;
		for (const auto& DirContent : std::filesystem::directory_iterator{std::filesystem::path(InspectingDir)})
		{

		    std::string MatchedFile = DirContent.path().filename().string();
			if (MatchedFile.substr(0, LastWordLen) == std::string(LastWord))
			{
              if (std::filesystem::is_directory(std::filesystem::path(MatchedFile)))
                FileMatch.push_back(MatchedFile + "/");
              else
			    FileMatch.push_back(MatchedFile);              
			} 	
		}

		if (FileMatch.size() == 1)
		{
			std::string RemainingStr = FileMatch[0].substr(LastWordLen);
			std::cout << RemainingStr <<std::flush;
			strcat(Input, RemainingStr.c_str());
			NoOfCharTyped += RemainingStr.size();
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
		  	  	  strcat(Input, RemainingCommonStr.c_str());
		  	  	  std::cout <<RemainingCommonStr <<std::flush;
		  	  	  NoOfCharTyped += i - LastWordLen;
		  	  	  break;
		  	  	}
		  	  }
		  	
		  }

		  // prints the matched files in the next line, and then prints the prompt and the
		  // written command again for the user to complete the command
          printf("\n\r");
		  fflush(stdout);

		  for (const std::string& MatchedFile : FileMatch)
		    std::cout <<"  " <<MatchedFile <<std::flush;

		  PrintPrompt();
		  printf("%s", Input);
		  fflush(stdout);

		  
		}

		 		
		
		
        
        break;
      }

        


    };

  }
}
