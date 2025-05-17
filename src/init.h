#pragma once

#define MAX_INPUT 1024

extern char* token[MAX_INPUT];
extern int argc;
extern char User[MAX_INPUT];
extern char CWD[MAX_INPUT];

void InitTermianal();
void PrintPrompt();
void ReadInput(char* Input, int Length);
// void Parse();
void Parse(char *Input);
void Execute();
