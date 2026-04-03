#include "parse_and_execute.h"
#include "raw_mode.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>


void Parse(std::string& Input, std::vector<std::string>& argv)
{

  if (Input == "")
    return;


  
  int BufferPtr = 0;
  int BuffStartIndex = 0;
  for (int i = 0; Input[i] != '\0'; i++)
  {
    if (Input[i] == ' ')
    {
      if (BufferPtr == 0)
        continue;
        
      argv.push_back(Input.substr(BuffStartIndex, BufferPtr)); // pushing the part of the input which is an argument
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
    
  }
  
  
}



int Execute(std::vector<std::string>& argv)
{


  if (argv.size() == 0)
    return NORMAL_EXIT;


  if (argv[0] == "exit")
    return PROGRAMME_EXIT;


  else if (argv[0] == "cd")
  {

  
  	if (argv.size() == 1)
  	{
      std::cerr << "Vx: No file name mentioned for cd\n\r";
      return USER_ERROR_EXIT;
  	}
  
    std::error_code ec;
    std::filesystem::current_path(argv[1], ec);

    if (ec)
    {
    
      std::cerr <<"Vx: Cannot cd into " << argv[1] <<"\n\r";
      return USER_ERROR_EXIT;
      
    }

	CWD = std::filesystem::current_path();


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


  

  pid_t pid = fork();

  if (pid < 0)
    std::perror("Vx: Error, resource unavailable currently\n\r");

  else if (pid == 0)
  {

    execvp(CstyleArgv[0], CstyleArgv.data());

    std::cerr << "Vx: Cannot identify the command " << argv[0] <<"\n\r";
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

