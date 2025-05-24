#include "process.h"
#include "init.h"
#include "raw_mode.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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

  if (argv[0] == NULL)
    return;

  if (strcmp(argv[0], "exit") == 0)
    exit(1);

  else if (strcmp(argv[0], "cd") == 0)
  {

    if (argv[1] == NULL)
      fprintf(stderr, "Vx: NO file name mentioned for cd");


    std::error_code ec;
    std::filesystem::current_path(argv[1], ec);

    if (ec)
      fprintf(stderr, "Vx: cannot cd into %s\n\r", argv[1]);


    return;

  }

  DisableRawMode();


  pid_t pid = fork();

  if (pid < 0)
    perror("Vx: Error, resource unavailable currently");

  else if (pid == 0) {

    execvp(argv[0], argv);

    fprintf(stderr, "Vx: cannot identify the command %s", argv[0]);
    exit(EXIT_FAILURE);
  } 
  else {
      // Parent process
      int status;
      waitpid(pid, &status, 0);
  }


  EnableRawMode();

}

