#ifndef SFCOMMON_H__
#define SFCOMMON_H__

#ifndef SAMPFUNCS_API
#	ifdef SAMPFUNCS_EXPORTS
#		define SAMPFUNCS_API __declspec(dllexport)
#	else
#		define SAMPFUNCS_API __declspec(dllimport)
#	endif
#endif

class CPlugin;
class SFPluginInfo;
class SFRakNet;
class SFSAMP;
class SFRender;
class SFGame;
class SFCLEO;

typedef CPlugin* PPLUGIN;

#endif // SFCOMMON_H__
