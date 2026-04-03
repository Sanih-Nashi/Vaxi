#include "init.h"
#include "input.h"
#include "parse_and_execute.h"

#include <string>
#include <vector>


int main()
{

  std::string Input;
  std::vector<std::string> argv;

  InitTermianal();  
  while(true)
  { 
  
    PrintPrompt();
    
    if (ReadInput(Input) == EXIT_FAILURE)
      continue;
      
    Parse(Input, argv);
    Input.clear();
        
    if (Execute(argv) == PROGRAMME_EXIT)
      break;
    argv.clear();      
  }



  return 0;
  
}
