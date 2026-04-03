#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>


#include <unistd.h>
#include <sys/wait.h>


#include "init.h"
#include "raw_mode.h"
#include "definitions.h"


void InitTermianal()
{
  User = std::getenv("USER");  
  CWD = std::filesystem::current_path();
  ListDirContents(DirContents, CWD);
  
  // printf("\033[H\033[2J"); //clears screen and puts cursor on top
  std::cout <<"\033[H\033[2J";


  InitRawMode();
  EnableRawMode();

}

void PrintPrompt()
{

  std::cout <<"\r\n\033[34m" << User <<"\033[0m&\033[34mVx\033[0m::\n\r::\033[32m";


  for (int i = 0; CWD[i] != '\0'; i++)
  {

    if (CWD[i] == '/')
    {
      std::cout <<"\033[0m/\033[32m";
      continue;
    }


    std::cout <<CWD[i];
  }

  std::cout << "\033[0m->" <<std::flush;
  
}

