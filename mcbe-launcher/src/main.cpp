#include "utils.h"

using namespace std::chrono_literals;

int main()
{
    system("start Minecraft://");

    std::this_thread::sleep_for(1s);

    DWORD pid = utils::find_pid("Minecraft.Windows.exe");
    if (pid == 0ul)
    {
        std::cout << "Minecraft not running" << std::endl;
        return 1;
    }

    ULONGLONG base = utils::GetModuleBase(pid, "Minecraft.Windows.exe");
    std::cout << std::hex << base << std::dec << std::endl;

    auto gameHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (gameHandle == INVALID_HANDLE_VALUE) {
        std::cout << "Cannot OPEN handle to minecraft" << std::endl;
        std::cin.get();
        return 1;
    }

    constexpr auto STORE_OWNERSHIP_CHECK_OFFSET = 0xDF79F6;
    SIZE_T written{};
    char buf[4]{};
    buf[0] = -21;
    if (!WriteProcessMemory(gameHandle, (LPVOID)(base + STORE_OWNERSHIP_CHECK_OFFSET), buf, 1, &written))
        std::cout << "Failed to patch mem" << std::endl;
    if (written != 1)
        std::cout << "Failed to patch mem" << std::endl;

    std::cout << "Success" << std::endl;
    std::cin.get();
    
    return 0;
}