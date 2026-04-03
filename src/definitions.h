#pragma once


#include <vector>
#include <string>



#define MAX_INPUT 1024

// keys
#define ENTER_KEY 13
#define BACKSPACE 8
#define DEL_KEY 127
#define SPACE_KEY 32
#define TAB 9

// Function return values

#define PROGRAMME_EXIT 1 // returning this value will result in termination
#define USER_ERROR_EXIT 0 // as we just reset the terminal in this case
#define NORMAL_EXIT 0



// the number of characters typed
inline int NoOfCharTyped;
inline bool InsideSingleQuotes;
inline bool InsideDoubleQuotes;

inline std::string CWD;

inline std::string User;
inline std::string Input;


inline std::vector<std::string> DirContents;

// List the contents of the current dir
void ListDirContents(std::vector<std::string>& DirFile, const std::string& WorkingDir);

// gives the last word of a string along with the RelativePath
bool LastWordAndRelativePath(char* str, char** LastWord, char** RelativeDirPath);
