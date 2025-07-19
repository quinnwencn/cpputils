//
// Created by quinn on 7/19/2025.
// Copyright (c) 2025 All rights reserved.
//

#include <array>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "cpputils/shell.h"

namespace Cpputils {

int Shell(std::string_view cmd) {
	std::string _output;
	return Shell(cmd, _output);
}

int Shell(std::string_view cmd, std::string& output, bool includeStderr) {
#ifdef _WIN32
	SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    HANDLE hStdoutRead, hStdoutWrite;
    if (!CreatePipe(&hStdoutRead, &hStdoutWrite, &sa, 0)) {
        return -1;
    }
    if (!SetHandleInformation(hStdoutRead, HANDLE_FLAG_INHERIT, 0)) {
        return -1;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = hStdoutWrite;
    si.hStdError = includeStderr ? hStdoutWrite : nullptr;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    // Create process
    std::string cmdStr(cmd);
    if (!CreateProcessA(nullptr, cmdStr.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi)) {
        CloseHandle(hStdoutWrite);
        CloseHandle(hStdoutRead);
        return -1;
    }

    CloseHandle(hStdoutWrite);

    // Read output
    output.clear();
    char buffer[4096];
    DWORD bytesRead;
    while (ReadFile(hStdoutRead, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0) {
        output.append(buffer, bytesRead);
    }

    CloseHandle(hStdoutRead);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return static_cast<int>(exitCode);
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
