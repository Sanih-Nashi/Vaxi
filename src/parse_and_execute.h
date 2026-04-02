#pragma once

#include <cstddef>
#include "init.h"

inline char* argv[MAX_INPUT] = {NULL};
inline int argc;

void Parse(char *Input);
int Execute();
