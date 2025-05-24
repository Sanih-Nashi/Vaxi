#include "init.h"
#include "input.h"
#include "process.h"

#include <stdlib.h>


int main()
{
  InitTermianal();
  while(true)
  { 
    PrintPrompt();
    if (ReadInput(Input, MAX_INPUT) == EXIT_FAILURE)
      continue;
    Parse(Input);
    Execute();
  }
}
