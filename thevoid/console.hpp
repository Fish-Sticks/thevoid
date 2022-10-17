#pragma once
#include <Windows.h>
#include <iostream>


namespace console
{
	static FILE* f[3]{};

	void create_console()
	{
		AllocConsole();
		freopen_s(&f[0], "CONOUT$", "w", stdout);
		freopen_s(&f[1], "CONIN$", "r", stdin);
		freopen_s(&f[2], "CONOUT$", "w", stderr);

		SetConsoleTitleA("The void");
	}

	void free_console()
	{
		for (std::uint32_t i = 0; i < 3; ++i)
		{
			fclose(f[i]);
		}
		FreeConsole();
	}
}