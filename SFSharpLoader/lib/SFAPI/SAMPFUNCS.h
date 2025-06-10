#ifndef SAMPFUNCS_H__
#define SAMPFUNCS_H__

#include "SFCommon.h"
#include "SFCommand.h"
#include <windows.h>
#include <string>
#include <vector>

class SAMPFUNCS_API SAMPFUNCS {
public:
	PPLUGIN pPlugin;

	class SFRakNet* getRakNet(void);
	class SFSAMP* getSAMP(void);
	class SFRender* getRender(void);
	class SFGame* getGame(void);
	class SFCLEO* getCLEO(void);

	void Log(const char* pText, ...);
	void LogFile(const char* pText, ...);
	void LogConsole(const char* pText, ...);
	bool initPlugin(void(__stdcall* PluginThread)(void), HMODULE hModule);
	unsigned int getAPIVersion(void);
	unsigned int getSFVersion(void);
	void registerConsoleCommand(std::string cmd, CommandProc callback);
	void unregisterConsoleCommand(std::string cmd);
	void execConsoleCommand(std::string command);
	bool isConsoleOpened(void);
	bool isPluginLoaded(std::string strPluginName);
	void loadPlugin(std::string strPluginName);
	void unloadPlugin(std::string strPluginName);
	std::vector<stCommandInfo> getChatCommands(void);
	std::vector<stCommandInfo> getConsoleCommands(void);
	void setConsoleCommandDescription(std::string command, std::string description);
	void Log(const std::string& pText);
};

#endif
