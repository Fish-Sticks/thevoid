#include <Windows.h>
#include <thread>

#include "console.hpp"
#include "rendering.hpp"

void cleanup(HMODULE hModule);

DWORD main_thread(HMODULE hModule)
{
    console::create_console();

    rendering::initiate_render_hook();

    cleanup(hModule);
    return ERROR_SUCCESS;
}

void cleanup(HMODULE hModule)
{
    std::cin.get();
    rendering::uninitiate_render_hook();
    console::free_console();
    FreeLibraryAndExitThread(hModule, ERROR_SUCCESS);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            CloseHandle(CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread), hModule, NULL, NULL));
            break;
        }
    }
    return TRUE;
}

