#include <stdio.h>
#include <filesystem>
#include <cstdlib>
#include <cstring>

#ifdef __unix__

#include <unistd.h>
#include <sys/wait.h>

#elif defined(WIN32) || defined(WIN64)

#include <io.h>
#include <windows.h>

#endif

#include "init.h"



void InitTermianal()
{
  printf("\033[H\033[2J"); //clears screen and puts cursor on top

#ifdef __unix__

  User = std::getenv("USER");

#elif defined(WIN32) || defined(WIN64)

  User = std::getenv("USERNAME");

#endif
  strcpy(CWD, std::filesystem::current_path().c_str());


}

void PrintPrompt()
{

  strcpy(CWD, std::filesystem::current_path().c_str());


  printf("\n\033[34m%s\033[0m&\033[34mVx\033[0m::\n::\033[32m", User);

  for (int i = 0; CWD[i] != '\0'; i++)
  {

#ifdef __unix__
    if (CWD[i] == '/')
    {
      printf("\033[0m/\033[32m");
      continue;
    }


#elif defined(WIN32) || defined(WIN64)
    if (CWD[i] == '\\')
    {
      printf("\033[0m\\\033[35m");
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

    char str[MAX_INPUT];
    sprintf(str, "Vx: %s", token[0]);
    perror(str);
    exit(EXIT_FAILURE);
  } 
  else {
      // Parent process
      int status;
      waitpid(pid, &status, 0);
  }

}


#elif defined(WIN32) || defined(WIN64)


void Execute();
{
  char* cmdLineStr = buildCommandLine(argv);
  if (!cmdLineStr) {
      fprintf(stderr, "Failed to build command line\n");
      return -1;
  }

  STARTUPINFOA si = {0};
  PROCESS_INFORMATION pi = {0};
  si.cb = sizeof(si);

  BOOL success = CreateProcessA(
      NULL,               // No module name
      cmdLineStr,          // Command line string
      NULL, NULL,          // Security attributes
      FALSE,               // Inherit handles
      0,                   // Creation flags
      NULL, NULL,         // Environment and current directory
      &si, &pi            // Pointers to structures
  );

  if (!success) {
      fprintf(stderr, "CreateProcess failed: error %lu\n", GetLastError());
      free(cmdLineStr);    // Assuming buildCommandLine allocates memory
      return -1;
  }

  // Wait for process to complete
  WaitForSingleObject(pi.hProcess, INFINITE);

  DWORD exitCode = 0;
  GetExitCodeProcess(pi.hProcess, &exitCode);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  free(cmdLineStr);        // Free the allocated command line string

}

#endif




