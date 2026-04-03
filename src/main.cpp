#include "init.h"
#include "input.h"
#include "parse_and_execute.h"

#include <stdlib.h>


int main()
{


  InitTermianal();

  while(true)
  { 
  
    PrintPrompt();
    
    if (ReadInput(Input) == EXIT_FAILURE)
      continue;
      
    Parse(Input);
    
    if (Execute() == PROGRAMME_EXIT)
      break;
      
  }



  return 0;
  
}
