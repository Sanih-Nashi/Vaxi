#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>

#ifdef __unix__

#include <unistd.h>
#include <sys/wait.h>

#elif defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)

#include <windows.h>

#endif

#include "init.h"



void InitTermianal()
{
  printf("\033[H\033[2J"); //clears screen and puts cursor on top

#ifdef __unix__

  User = std::getenv("USER");
  strcpy(CWD, std::filesystem::current_path().c_str());

#elif defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)

  User = std::getenv("USERNAME");
  strcpy(CWD, std::filesystem::current_path().string().c_str());

  #endif


}

void PrintPrompt()
{

#ifdef __unix__

  strcpy(CWD, std::filesystem::current_path().c_str());

#elif defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)

  strcpy(CWD, std::filesystem::current_path().string().c_str());

#endif

  printf("\n\033[34m%s\033[0m&\033[34mVx\033[0m::\n::\033[32m", User);

  for (int i = 0; CWD[i] != '\0'; i++)
  {

#ifdef __unix__
    if (CWD[i] == '/')
    {
      printf("\033[0m/\033[32m");
      continue;
    }


#elif defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
    if (CWD[i] == '\\')
    {
      printf("\033[0m\\\033[32m");
      continue;
    }
    if (CWD[i] == ':')
    {
      printf("\033[0m:\033[32m");
      continue;
    }
#endif

    printf("%c", CWD[i]);
  }
  printf("\033[0m->");
  fflush(stdout);
  
}

void ReadInput(char* Input, int Length)
{
  if (fgets(Input, Length, stdin) == nullptr)
  {
    perror("\nVx: Interrupt, exited with error code 1");
    exit(1);
  }
  
}


void Parse(char* Input)
{
  Input[strcspn(Input, "\n")] = '\0';
    if (Input == NULL)
        return;

    argc = 0;
    token[argc] = strtok(Input, " ");
    while (token[argc] != NULL && argc < MAX_INPUT - 1)
    {
        argc++;
        token[argc] = strtok(NULL, " ");
    }
    token[argc] = NULL;
}

void Execute()
{

  if (token[0] == NULL)
    return;

  else if (strcmp(token[0], "exit") == 0)
    exit(1);

  else if (strcmp(token[0], "cd") == 0)
  {

    if (token[1] == NULL)
      fprintf(stderr, "Vx: NO file name mentioned for cd");


    std::error_code ec;
    std::filesystem::current_path(token[1], ec);

    if (ec)
      fprintf(stderr, "Vx: cannot cd into %s\n", token[1]);


    return;

  }


#ifdef __unix__


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



#elif defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)

  STARTUPINFOA si = { sizeof(STARTUPINFOA) };
  PROCESS_INFORMATION pi = { 0 };

  BOOL success = CreateProcessA(
    NULL,
    Input,
    NULL,
    NULL,
    FALSE,
    0,
    NULL,
    NULL,
    &si,
    &pi
  );

  if (!success)
  {
    fprintf(stderr, "Vx: cannot identify the command %s", token[0]);
    return;
  }

  // Wait for process to finish
  WaitForSingleObject(pi.hProcess, INFINITE);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

#endif


}

