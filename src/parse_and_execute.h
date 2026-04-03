#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include "init.h"

inline std::vector<std::string> argv;
inline int argc;

void Parse(std::string& Input);
int Execute();
