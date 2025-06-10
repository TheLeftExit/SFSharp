#include "plugin.h"
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH) {
		return PluginInit(hModule);
	}
	return TRUE;
}
