//
// Created by quinn on 7/19/2025.
// Copyright (c) 2025 All rights reserved.
//

#include <array>
#include <string>

#ifndef _WIN32
#include <unistd.h>
#endif
#include <cstdio>

#include "cpputils/shell.h"

namespace Cpputils {

int Shell(std::string_view cmd) {
	std::string _output;
	return Shell(cmd, _output);
}

int Shell(std::string_view cmd, std::string& output, bool includeStderr) {
#ifdef _WIN32
	FILE* pipe = _popen(cmd.data(), "r");
	if (pipe == nullptr) {
		if (includeStderr) {
			output = "popen() failed!";
		}
		return 1;
	}

	std::array<char, 128> buffer;
	while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
		output += buffer.data();
	}

	int exitCode = _pclose(pipe);
	if (exitCode == -1) {
		output += "\nFailed to close pipe or get return code.";
	}

    return exitCode;
#else
	std::string command {cmd};
	if (includeStderr) {
		command += " 2>&1";
	}

	std::array<char, 128> buffer;
	FILE* pipe = popen(command.c_str(), "r");
	if (pipe == nullptr) {
		output = "popen() failed!";
		return -1;
	}

	while (feof(pipe) == 0) {
		if (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
			output += buffer.data();
		}
	}

	int exitCode = pclose(pipe);
	return WEXITSTATUS(exitCode);
#endif
}

}
