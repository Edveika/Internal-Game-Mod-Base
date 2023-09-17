#include <stdio.h>
#include <Windows.h>

// Entry point that gets called then the DLL gets attached to the process
DWORD WINAPI OnDllAttach(HMODULE hModule)
{
#ifdef DEBUG
	// Allocate the console
	AllocConsole();
	// f file
	FILE* f;
	// Open f file
	freopen_s(&f, "CONOUT$", "w", stdout);
#endif

#ifdef DEBUG
	// Close f file
	fclose(f);
	// Close the console
	FreeConsole();
	// Unload the module
	FreeLibraryAndExitThread(hModule, 0);
#endif

	// Return 0
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// Calls OnDllAttach when dll gets injected into game's memory
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)OnDllAttach, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}