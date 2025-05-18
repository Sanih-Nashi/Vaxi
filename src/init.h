#pragma once

#define MAX_INPUT 1024

inline char* token[MAX_INPUT];
inline int argc;
inline char* User;
inline char CWD[MAX_INPUT];

void InitTermianal();
void PrintPrompt();
void ReadInput(char* Input, int Length);
// void Parse();
void Parse(char *Input);
void Execute();
