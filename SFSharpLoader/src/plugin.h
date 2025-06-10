#pragma once

#include <SAMPFUNCS_API.h>
#include <windows.h>
#include <memory>

extern std::unique_ptr<SAMPFUNCS> SF;

bool PluginInit(HMODULE hModule);
