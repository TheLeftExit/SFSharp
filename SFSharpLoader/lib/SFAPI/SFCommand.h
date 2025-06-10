#ifndef SFCOMMAND_H__
#define SFCOMMAND_H__

#include "SFCommon.h"
#include <string>

typedef void(__stdcall* CommandProc)(std::string params);

#pragma warning(push)
#pragma warning(disable : 4251)

struct SAMPFUNCS_API stCommandInfo {
	enum CommandType {
		NONE,
		SCRIPT,
		PLUGIN
	};

	stCommandInfo(std::string, CommandType, void*);

	std::string name;
	CommandType type;
	union {
		struct CScriptThread* thread;
		class SFPluginInfo* plugin;
		void* owner;
	};
};

#pragma warning(pop)

#endif // SFCOMMAND_H__
