#ifndef SFPLUGIN_H__
#define SFPLUGIN_H__

#include "SFCommon.h"
#include <windows.h>
#include <string>

#pragma warning(push)
#pragma warning(disable : 4251)

class SAMPFUNCS_API SFPluginInfo {
public:
	SFPluginInfo(HMODULE, std::string);
	HMODULE getPluginHandle(void);
	std::string getPluginName(void);

private:
	HMODULE m_hPlugin;
	std::string m_strPluginName;
};

#pragma warning(pop)

#endif // SFPLUGIN_H__
