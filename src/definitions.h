#pragma once

#define MAX_INPUT 1024
#define ENTER_KEY 13
#define BACKSPACE 8
#define DEL_KEY 127
#define SPACE_KEY 32
#define TAB 9

#include <vector>
#include <string>

// the number of characters typed
inline int NoOfCharTyped;
inline bool InsideSingleQuotes;
inline bool InsideDoubleQuotes;
inline char CWD[MAX_INPUT];
inline std::vector<std::string> DirContents;

// List the contents of the current dir
void ListDirContents(std::vector<std::string>& DirFile, const std::string& WorkingDir);

// gives the last word of a cstringy
char* LastWord(char* str);
