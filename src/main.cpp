#include "init.h"
#include <stdio.h>


int main()
{
  InitTermianal();
  while(true)
  { 
    PrintPrompt();
    ReadInput(Input, MAX_INPUT);
    Parse(Input);
    Execute();
  }
}