#pragma once 


#include <termios.h>

extern struct termios OrigSettings;
extern struct termios RawModeSettings;


void InitRawMode();
void EnableRawMode();
void DisableRawMode();