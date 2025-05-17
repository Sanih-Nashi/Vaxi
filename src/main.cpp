#include "init.h"
#include <stdio.h>


int main()
{
  char Input[MAX_INPUT];
  InitTermianal();
  while(true)
  { 
    PrintPrompt();
    ReadInput(Input, MAX_INPUT);
    Parse(Input);
    Execute();
  }
}