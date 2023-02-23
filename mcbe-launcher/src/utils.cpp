#include "utils.h"
#include <TlHelp32.h>
#include <Psapi.h>

namespace utils
{
    DWORD find_pid(const std::string& process_name)
    {
        HANDLE processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W proc{};
        proc.dwSize = sizeof(PROCESSENTRY32W);
        if (!Process32FirstW(processes, &proc))
        {
            std::wcout << std::hex << GetLastError() << std::endl;
            return 0;
        }
        DWORD pid = 0;

        do
        {
            if (!wcscmp(proc.szExeFile, L"Minecraft.Windows.exe"))
            {
                pid = proc.th32ProcessID;
                break;
            }
        } while (Process32NextW(processes, &proc));

        CloseHandle(processes);
        return pid;
    }

    ULONGLONG GetModuleBase(DWORD pid, const std::string& dll_name)
    {
        ULONGLONG base{};
        auto modules = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
        if (modules != INVALID_HANDLE_VALUE)
        {
            MODULEENTRY32 mod{};
            mod.dwSize = sizeof(MODULEENTRY32);
            if (Module32First(modules, &mod))
            {
                do
                {
                    if (!strcmp(mod.szModule, dll_name.c_str()))
                    {
                        base = (ULONGLONG)mod.modBaseAddr;
                        break;
                    }
                } while (Module32Next(modules, &mod));
            }
        }
        return base;
    }
}