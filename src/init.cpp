#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>


#include <unistd.h>
#include <sys/wait.h>


#include "init.h"
#include "raw_mode.h"


void InitTermianal()
{
  printf("\033[H\033[2J"); //clears screen and puts cursor on top


  User = std::getenv("USER");
  strcpy(CWD, std::filesystem::current_path().c_str());
  
  InitRawMode();
  EnableRawMode();

}

void PrintPrompt()
{


  strcpy(CWD, std::filesystem::current_path().c_str());


  printf("\n\033[34m%s\033[0m&\033[34mVx\033[0m::\n\r::\033[32m", User);

  for (int i = 0; CWD[i] != '\0'; i++)
  {

    if (CWD[i] == '/')
    {
      printf("\033[0m/\033[32m");
      continue;
    }


    printf("%c", CWD[i]);
  }
  printf("\033[0m->");
  fflush(stdout);
  
}

