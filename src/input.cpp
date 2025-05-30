#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <vector>
#include <iostream>
#include <cstring>


#include "input.h"
#include "init.h"
#include "definitions.h"


char ReadKey(){
  int read_num;
  char c;
  while ( (read_num = read(STDIN_FILENO, &c, 1)) != 1) 
  {
    if (read_num == -1 && errno != EAGAIN)
    {
      perror("read");
      exit(1);
    }
  }
  return c;
}


int ReadInput(char* Input, int MaxLength)
{
  char c;

  while (true)
  {

    c = ReadKey();
    
    // visible characters
    if (32 <= c && c <= 126)
    {
      printf("%c", c);
      fflush(stdout);
      NoOfCharTyped++;

      if (NoOfCharTyped < MaxLength)
        Input[NoOfCharTyped - 1] = c;

      continue;
    }

    switch (c)
    {

      case ENTER_KEY:
      {
        printf("\n\r");
        fflush(stdout);

        if (NoOfCharTyped >= MaxLength)
        {
          NoOfCharTyped = 0;
          fprintf(stderr, "Vx: The command typed exceded the max characters of 1024\n\r    Try again with fewer lines\n\r");
          return EXIT_FAILURE;
        }

        Input[NoOfCharTyped] = '\0';
        NoOfCharTyped = 0;
        return EXIT_SUCCESS;
      }

      case BACKSPACE:
      case DEL_KEY:
      {
        if (NoOfCharTyped > 0)
        {

          printf("\b \b");
          fflush(stdout);
          NoOfCharTyped--;
          break;
        }
      }
      case TAB:
      {
        Input[NoOfCharTyped] = '\0';
        char* Last = LastWord(Input);
        int LastLen = strlen(Last);
        std::vector<int> Match; // stores the index value which matches
        if (LastLen == 0)
          break;
        for (int i = 0; i < DirContents.size(); i++)
        {
          if (DirContents[i].substr(0, LastLen) == Last)
            Match.push_back(i);
        }
        if (Match.size() < 1)
          break;
        else if (Match.size() == 1)
        {
          fprintf(stdout, "\033[%dD", LastLen);
          fflush(stdout);
          std::cout << DirContents[Match[0]] << std::flush;
	  strcpy(Last, DirContents[Match[0]].c_str());
          NoOfCharTyped += DirContents[Match[0]].size() - LastLen;
        }
        break;
      }

    };

  }

}
