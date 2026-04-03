#pragma once


#include <vector>
#include <string>


// keys defined by their ascii
#define ENTER_KEY 13
#define BACKSPACE 8
#define DEL_KEY 127
#define SPACE_KEY 32
#define TAB 9

// Function return values

#define PROGRAMME_EXIT 1 // returning this value will result in termination
#define USER_ERROR_EXIT 0 // as we just reset the terminal in this case
#define NORMAL_EXIT 0



inline std::string CWD;

inline std::string User;


// gives the last word of a string along with the RelativePath
// bool LastWordAndRelativePath(char* str, char** LastWord, char** RelativeDirPath);
bool LastWordAndRelativePath(const std::string& str, std::string& LastWord, std::string& RelativeDirPath);
