#include "process.h"
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

static char buffer[MAX_INPUT];

void Parse(char* Input)
{

  if (Input == NULL)
    return;

  argc = 0;
  
  int BufferPtr = 0;
  int BufStrlen;
  int i = 0;
  for (; Input[i] != '\0'; i++)
  {
    if (Input[i] == ' ')
    {
      buffer[BufferPtr] = '\0';
      BufStrlen = strlen(buffer);
      if (BufStrlen == 0)
        continue;
      argv[argc] = (char*) realloc(argv[argc], BufStrlen + 1 + 1); // '+ 1' to include the null termination char
      strcpy(argv[argc++], buffer);
      BufferPtr = 0;
      continue;
    }

    else if (Input[i] == '\'')
    {
      i++;
      for (; Input[i] != '\'' && Input[i] != '\0'; i++)
        buffer[BufferPtr++] = Input[i];
        
      continue;
    }

    else if (Input[i] == '\"')
    {
      i++;
      for (; Input[i] != '\"' && Input[i] != '\0'; i++)
        buffer[BufferPtr++] = Input[i];
      continue;
    }

    else if (Input[i] == '\\')
      i++;

    buffer[BufferPtr++] = Input[i];
    
  }
  BufStrlen = strlen(buffer);
  buffer[BufferPtr] = '\0';
  if (BufStrlen != 0)
  {
    argv[argc] = (char*) realloc(argv[argc], BufStrlen + 1); // + 1 so that in includes the null termination char
    strcpy(argv[argc++], buffer);
  }
  argv[argc] = NULL;


}



void Execute()
{

  if (*argv[0] == '\0')
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
    {
      fprintf(stderr, "Vx: cannot cd into %s\n\r", argv[1]);
      return;
    }

    strcpy(CWD, std::filesystem::current_path().c_str());
    DirContents.clear();
    ListDirContents(DirContents, CWD);



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

