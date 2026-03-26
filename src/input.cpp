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
  //this is the dir from which we will check the daughter files of a specific dir mentioned by the user
  // it's main use case will be for describing the fuction of Tab key 


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
          break;
        }
      }



      case TAB:
      {
        Input[NoOfCharTyped] = '\0';

        char* LastWord;
        char* RelativePathDir;
        int LastWordLen;


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
		
			if (DirContent.path().filename().string().substr(0, LastWordLen) == std::string(LastWord))
			  FileMatch.push_back(DirContent.path().filename().string());
			  	
		}

		if (FileMatch.size() == 1)
		{
			std::string RemainingStr = FileMatch[0].substr(LastWordLen);
			if (std::filesystem::is_directory(std::filesystem::path(FileMatch[0])))
			  RemainingStr.push_back('/');
			std::cout << RemainingStr <<std::flush;
			strcat(Input, RemainingStr.c_str());
			NoOfCharTyped += RemainingStr.size();
		}

		 		
		
		
        
        break;
      }

        


    };

  }
}
