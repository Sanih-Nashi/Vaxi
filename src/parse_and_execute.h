#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include "init.h"


void Parse(std::string& Input, std::vector<std::string>& argv);
int Execute(std::vector<std::string>& argv);
