#include "plugin.h"
#include "exports.h"
#include <game_api.h>
#include <filesystem>

std::unique_ptr<SAMPFUNCS> SF;

static std::vector<CSharpLoopCallback> mainLoopCallbacks;

static void LoadModules() {
	std::filesystem::path moduleFolder = std::filesystem::current_path() / "SFSharp";
	if(!std::filesystem::exists(moduleFolder) || !std::filesystem::is_directory(moduleFolder)) {
		return;
	}
	std::vector<CSharpLoopCallback> callbacks;
	for (const auto& entry : std::filesystem::directory_iterator(moduleFolder)) {
		if(entry.is_regular_file() && entry.path().extension() == ".dll") {
			HINSTANCE hModule = LoadLibraryW(entry.path().c_str());
			CSharpExports exports;
			CSharpInitFunction init = reinterpret_cast<CSharpInitFunction>(GetProcAddress(hModule, "SFSharpMain"));
			int returnCode = init(&exports);
			switch (returnCode) {
				case 0:
					callbacks.push_back(exports.loopCallback);
					continue;
				case 1:
					std::string errorMessage = "SFSharp plugin version mismatch: " + entry.path().filename().string();
					SF->getSAMP()->getChat()->AddChatMessage(0xFFAAAAAA, errorMessage.c_str());
					continue;
			}
			
		}
	}
	mainLoopCallbacks = callbacks;
}

static void CALLBACK mainloop() {
	static bool initialized = false;
	if (!initialized && GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized()) {
		initialized = true;
		LoadModules();
		SF->getSAMP()->getChat()->AddChatMessage(0xFFAAAAAA, "SFSharp initialized. Plugins loaded: %i", mainLoopCallbacks.size());
		return;
	}
	if(initialized) {
		for (const CSharpLoopCallback& callback : mainLoopCallbacks) {
			callback();
		}
	}
}

bool PluginInit(HMODULE hModule) {
	SF = std::make_unique<SAMPFUNCS>();
	SF->initPlugin(&mainloop, hModule);
	return true;
}
