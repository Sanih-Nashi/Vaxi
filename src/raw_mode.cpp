#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "raw_mode.h"



struct termios OrigSettings;
struct termios RawModeSettings;



void InitRawMode()
{
    
  if (tcgetattr(STDIN_FILENO, &OrigSettings) == -1) 
  {
    perror("tcgetattr");
    exit(EXIT_FAILURE);
  }


  atexit(DisableRawMode);
  RawModeSettings = OrigSettings;

  RawModeSettings.c_lflag &= ~(ECHO | ISIG | IEXTEN | ICANON);
  RawModeSettings.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  RawModeSettings.c_oflag &= ~(OPOST);
  RawModeSettings.c_cflag |= (CS8);

  RawModeSettings.c_cc[VMIN] = 0;
  RawModeSettings.c_cc[VTIME] = 1;

}


void EnableRawMode()
{
  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &RawModeSettings) == -1)
  {
    perror("tcsetattr");
    exit(EXIT_FAILURE);
  }
}
void DisableRawMode()
{
  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &OrigSettings) == -1)
  {
    perror("tcsetattr");
    exit(EXIT_FAILURE);
  }

}
