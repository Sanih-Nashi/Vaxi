#include "parse_and_execute.h"
#include "init.h"
#include "raw_mode.h"
#include "definitions.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

//static char buffer[MAX_INPUT]; // just stores the input value then it is copied to argv[x]

void Parse(std::string& Input)
{

  if (Input == "")
    return;

  // used for Execute() to number out the number of arguments
  argc = 0;

  
  int BufferPtr = 0;
  int BuffStartIndex = 0;
  for (int i = 0; Input[i] != '\0'; i++)
  {
    if (Input[i] == ' ')
    {
      if (BufferPtr == 0)
        continue;
        
      argv.push_back(Input.substr(BuffStartIndex, BufferPtr)); // pushing the part of the input which is an argument
      argc++; //TODO: remove argc completely
      BuffStartIndex = i + 1; // because i points to the space we will make the new start index i + 1
      BufferPtr = 0;
      
      continue;
    }

    else if (Input[i] == '\'')
    {
      Input.erase(i, 1);
      for (; Input[i] != '\'' && Input[i] != '\0'; i++)
        BufferPtr++;
      if (Input[i] == '\'')
        Input.erase(i, 1);
        
        
      continue;
    }

    else if (Input[i] == '\"')
    {
    
      Input.erase(i, 1);
      
      for (; Input[i] != '\"' && Input[i] != '\0'; i++)
        BufferPtr++;
        
      if (Input[i] == '\"')
        Input.erase(i, 1);
      
      continue;
      
    }

    else if (Input[i] == '\\')
      Input.erase(i, 1);

    BufferPtr++;
    
  }

  
  if (BufferPtr != 0)
  {
  
    argv.push_back(Input.substr(BuffStartIndex, BufferPtr));
    argc++;
    
  }
  
  Input.clear();  //TODO: place the initialization and clear() of input inside the main()
  
}



int Execute()
{


  if (argv.size() == 0)
  {
    argv.clear();
    return NORMAL_EXIT;
  }


  if (argv[0] == "exit")
  { 
    argv.clear();
    return PROGRAMME_EXIT;
  }


  else if (argv[0] == "cd")
  {

  
  	if (argv.size() == 1)
  	{
      std::cerr << "Vx: No file name mentioned for cd";
      argv.clear();
      return USER_ERROR_EXIT;
  	}
  
    std::error_code ec;
    std::filesystem::current_path(argv[1], ec);

    if (ec)
    {
    
      std::cerr <<"Vx: Cannot cd into " << argv[1] <<"\n\r";
      argv.clear();
      return USER_ERROR_EXIT;
      
    }

	CWD = std::filesystem::current_path();
	DirContents.clear();
	ListDirContents(DirContents, CWD);


    argv.clear();
    return NORMAL_EXIT;

  }



  // executing the command

  DisableRawMode();

  // changing argv into a cstyle char* array
  std::vector<char*> CstyleArgv;
  CstyleArgv.reserve(argv.size() + 1);
  
  for (const std::string& str : argv)
    CstyleArgv.emplace_back(const_cast<char*>(str.c_str()));
  CstyleArgv.push_back(NULL);

  argv.clear();

  

  pid_t pid = fork();

  if (pid < 0)
    std::perror("Vx: Error, resource unavailable currently");

  else if (pid == 0)
  {

    execvp(CstyleArgv[0], CstyleArgv.data());

    std::cerr << "Vx: Cannot identify the command " << argv[0];
    return PROGRAMME_EXIT;
  } 
  else 
  {
      // Parent process
      int status;
      waitpid(pid, &status, 0);
  }


  EnableRawMode();
  return NORMAL_EXIT;

}

