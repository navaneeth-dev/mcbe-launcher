#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>

namespace utils
{
    DWORD find_pid(const std::string& process_name);
    ULONGLONG GetModuleBase(DWORD pid, const std::string& dll_name);
}