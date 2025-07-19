//
// Created by quinn on 7/19/2025.
// Copyright (c) 2025 All rights reserved.
//

#ifndef SHELL_H
#define SHELL_H

#include <string_view>

namespace Cpputils {

int Shell(std::string_view cmd);
int Shell(std::string_view cmd, std::string& output, bool includeStderr = false);

}

#endif //SHELL_H
