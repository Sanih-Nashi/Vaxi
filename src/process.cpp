#include "process.h"
#include "init.h"
#include "raw_mode.h"

#include <stdio.h>

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>



void Parse(char* Input)
{

  if (Input == NULL)
    return;

  argc = 0;
  argv[argc] = strtok(Input, " ");
  while (argv[argc] != NULL && argc < MAX_INPUT - 1)
  {
    argc++;
    argv[argc] = strtok(NULL, " ");
  }
}



void Execute()
{

  if (token[0] == NULL)
    return;

  if (strcmp(token[0], "exit") == 0)
    exit(1);

  else if (strcmp(token[0], "cd") == 0)
  {

    if (token[1] == NULL)
      fprintf(stderr, "Vx: NO file name mentioned for cd");


    std::error_code ec;
    std::filesystem::current_path(token[1], ec);

    if (ec)
      fprintf(stderr, "Vx: cannot cd into %s\n\r", token[1]);


    return;

  }

  DisableRawMode();


  pid_t pid = fork();

  if (pid < 0)
    perror("Vx: Error, resource unavailable currently");

  else if (pid == 0) {

    execvp(token[0], token);

    fprintf(stderr, "Vx: cannot identify the command %s", token[0]);
    exit(EXIT_FAILURE);
  } 
  else {
      // Parent process
      int status;
      waitpid(pid, &status, 0);
  }


  EnableRawMode();

}

