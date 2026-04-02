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

static char buffer[MAX_INPUT]; // just stores the input value then it is copied to argv[x]

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



int Execute()
{


  if (*argv[0] == '\0')
    return NORMAL_EXIT;



  if (strcmp(argv[0], "exit") == 0)
  {
  	for (int i = 0; i < MAX_INPUT; i++)
  	  free(argv[i]);
    return PROGRAMME_EXIT;
  }


  else if (strcmp(argv[0], "cd") == 0)
  {

    if (argv[1] == NULL)
    {
      fprintf(stderr, "Vx: NO file name mentioned for cd");
	  return USER_ERROR_EXIT;
	}

    std::error_code ec;
    std::filesystem::current_path(argv[1], ec);

    if (ec)
    {
      fprintf(stderr, "Vx: cannot cd into %s\n\r", argv[1]);
      return USER_ERROR_EXIT;
    }

    strcpy(CWD, std::filesystem::current_path().c_str());
    DirContents.clear();
    ListDirContents(DirContents, CWD);



    return NORMAL_EXIT;

  }

  DisableRawMode();

  // from this point onwards if any issue arises on its readability, blame ChatGPT

  pid_t pid = fork();

  if (pid < 0)
    perror("Vx: Error, resource unavailable currently");

  else if (pid == 0) {

    execvp(argv[0], argv);

    fprintf(stderr, "Vx: cannot identify the command %s", argv[0]);
    return PROGRAMME_EXIT;
  } 
  else {
      // Parent process
      int status;
      waitpid(pid, &status, 0);
  }


  EnableRawMode();
  return NORMAL_EXIT;

}

