#ifndef SFSAMP_H_V__
#define SFSAMP_H_V__

#include "SFCommon.h"
#include "SFCommand.h"
#include <windows.h>
#include <stdint.h>
#include <string>

class RakClientInterface;
struct ID3DXFont;
struct ID3DXSprite;
struct ID3DXRenderToSurface;
struct IDirect3DTexture9;
struct IDirect3DSurface9;
struct IDirect3DDevice9;
struct RPCNode;

typedef DWORD D3DCOLOR;
typedef struct _D3DDISPLAYMODE D3DDISPLAYMODE;

enum DialogStyle {
	DIALOG_STYLE_MSGBOX = 0,
	DIALOG_STYLE_INPUT = 1,
	DIALOG_STYLE_LIST = 2,
	DIALOG_STYLE_PASSWORD = 3,
	DIALOG_STYLE_TABLIST = 4,
	DIALOG_STYLE_TABLIST_HEADERS = 5
};

enum Gamestate {
	GAMESTATE_NONE = 0,
	GAMESTATE_WAIT_CONNECT = 9,
	GAMESTATE_DISCONNECTED = 13,
	GAMESTATE_AWAIT_JOIN = 15,
	GAMESTATE_CONNECTED = 14,
	GAMESTATE_RESTARTING = 18
};

enum PlayerState {
	PLAYER_STATE_NONE = 0,
	PLAYER_STATE_ONFOOT = 17,
	PLAYER_STATE_PASSENGER = 18,
	PLAYER_STATE_DRIVER = 19,
	PLAYER_STATE_WASTED = 32,
	PLAYER_STATE_SPAWNED = 33
};

enum PlayerStatus : int {
	PLAYER_STATUS_TIMEOUT = 2,
};

enum Limits {
	SAMP_MAX_ACTORS = 1000,
	SAMP_MAX_PLAYERS = 1004,
	SAMP_MAX_VEHICLES = 2000,
	SAMP_MAX_PICKUPS = 4096,
	SAMP_MAX_OBJECTS = 1000,
	SAMP_MAX_GANGZONES = 1024,
	SAMP_MAX_3DTEXTS = 2048,
	SAMP_MAX_TEXTDRAWS = 2048,
	SAMP_MAX_PLAYERTEXTDRAWS = 256,
	SAMP_MAX_CLIENTCMDS = 144,
	SAMP_MAX_MENUS = 128,
	SAMP_MAX_PLAYER_NAME = 24,
	SAMP_ALLOWED_PLAYER_NAME_LENGTH = 20
};

enum ChatMessageType {
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT = 2,
	CHAT_TYPE_INFO = 4,
	CHAT_TYPE_DEBUG = 8
};

enum ChatDisplayMode {
	CHAT_WINDOW_MODE_OFF = 0,
	CHAT_WINDOW_MODE_LIGHT = 1,
	CHAT_WINDOW_MODE_FULL = 2
};

enum SendrateType {
	ONFOOTSENDRATE = 1,
	INCARSENDRATE = 2,
	AIMSENDRATE = 3
};

#pragma warning(push)
#pragma warning(disable : 4251)

#pragma pack(push, 1)

template<size_t S>
class SampStringBase {
	static_assert(S == 28u, "Unexpected std::string sizeof.");
};

template<>
class SampStringBase<24u> {
	void* containerProxy;
};

class SampString : private SampStringBase<sizeof(std::string)>, public std::string {
public:
	using std::string::string;
};

// structures
struct stSAMPPools {
	struct stActorPool* actorPool;
	struct stObjectPool* objectPool;
	struct stGangzonePool* gangzonePool;
	struct stTextLabelPool* textLabelPool;
	struct stTextdrawPool* textdrawPool;
	void* menuPool;
	struct stPlayerPool* playerPool;
	struct stVehiclePool* vehiclePool;
	struct stPickupPool* pickupPool;
};

struct SAMPFUNCS_API stSAMP {
	void* unk0;
	struct stServerInfo* serverInfo;
	uint8_t space[24];
	char serverIp[257];
	char serverName[259];
	bool nametagStatus; // changes by /nametagstatus
	uint32_t serverPort;
	uint32_t mapIcons[100];
	int lanMode;
	int gameState;
	uint32_t connectTick;
	struct stServerPresets* serverPresets;
	void* rakClientInterface;
	struct stSAMPPools* pools;

	// functions
	void RestartGame(void);
	short GetAnimationIDFromName(char* pszAnimFile, char* pszAnimName);
	char* GetAnimationNameFromID(short sAnimationID);
	void UpdateScoreAndPing(void);
	RPCNode* GetRPCNodeByIndex(uint8_t index);
};

struct stServerInfo {
	uint32_t ip;
	uint16_t port;
	// ...
};

struct stServerPresets {
	uint8_t cjWalk;
	uint8_t unk0[4];
	float worldBoundaries[4];
	uint8_t unk1;
	float gravity;
	uint8_t disableInteriorEnterExits;
	uint32_t vehicleFriendlyFire;
	uint8_t unk2[4];
	int classesAvailable;
	float nameTagsDistance;
	uint8_t unk3;
	uint8_t worldTimeHour;
	uint8_t worldTimeMinute;
	uint8_t weather;
	uint8_t noNametagsBehindWalls;
	uint8_t playerMarkersMode;
	uint8_t unk4[3];
	float globalChatRadiusLimit;
	uint8_t showNameTags;
};

struct stTextDrawTransmit {
	union {
		uint8_t flags;
		struct {
			uint8_t box : 1;
			uint8_t left : 1;
			uint8_t right : 1;
			uint8_t center : 1;
			uint8_t proportional : 1;
			uint8_t padding : 3;
		};
	};
	float letterWidth;
	float letterHeight;
	uint32_t letterColor;
	float boxWidth;
	float boxHeight;
	uint32_t boxColor;
	uint8_t shadow;
	uint8_t outline;
	uint32_t backgroundColor;
	uint8_t style;
	uint8_t selectable;
	float position[2];
	uint16_t modelId;
	float modelRotation[3];
	float modelZoom;
	uint16_t color[2];
};

struct stTextdraw {
	char text[800 + 1];
	char string[1600 + 2];
	float letterWidth;
	float letterHeight;
	uint32_t letterColor;
	uint8_t unk; // always = 01 (?)
	uint8_t center;
	uint8_t box;
	float boxSize[2];
	uint32_t boxColor;
	uint8_t proportional;
	uint32_t shadowColor;
	uint8_t shadowSize;
	uint8_t outline;
	uint8_t alignLeft;
	uint8_t alignRight;
	int style; // font style/texture/model
	float position[2];
	uint8_t gxt[8];
	uint32_t dword99B; // -1 by default
	uint32_t dword99F; // -1 by default
	uint32_t index;    // -1 if bad
	uint8_t selectable; // = 1; 0 by default
	uint16_t modelId;
	float modelRotation[3];
	float modelZoom;
	uint16_t color[2];
	uint8_t f9Be;
	uint8_t byte9Bf;
	uint8_t byte9C0;
	uint32_t dword9C1;
	uint32_t dword9C5;
	uint32_t dword9C9;
	uint32_t dword9Cd;
	uint8_t byte9D1;
	uint32_t dword9D2;
};

struct SAMPFUNCS_API stTextdrawPool {
	int globalIsListed[SAMP_MAX_TEXTDRAWS];
	int localIsListed[SAMP_MAX_PLAYERTEXTDRAWS];
	struct stTextdraw* globalTextdraw[SAMP_MAX_TEXTDRAWS];
	struct stTextdraw* localTextdraw[SAMP_MAX_PLAYERTEXTDRAWS];

	stTextdraw* Create(uint16_t id, stTextDrawTransmit* transmit, char* text);
	void Delete(uint16_t id);
	bool IsExists(uint16_t id);
	bool GetTransmit(uint16_t id, stTextDrawTransmit& transmit);
	struct stTextdraw* GetTextdraw(uint16_t id);
};

struct stPickup {
	int modelId;
	int type;
	float position[3];
};

struct stWeaponPickup {
	bool exists;
	uint16_t owner;
};

struct SAMPFUNCS_API stPickupPool {
	int largestId;
	uint32_t gtaPickupHandle[SAMP_MAX_PICKUPS];
	int id[SAMP_MAX_PICKUPS];
	int time[SAMP_MAX_PICKUPS];
	struct stWeaponPickup weaponPickup[SAMP_MAX_PICKUPS];
	struct stPickup pickup[SAMP_MAX_PICKUPS];

	// functions
	DWORD GetPickupHandleByID(int iID);
	int GetPickupIDByHandle(DWORD dwHandle);
	bool IsPickupExists(int iID);
};

struct SAMPFUNCS_API stPlayerPool {
	uint32_t maxPlayerId;
	uint16_t localPlayerId;
	SampString localPlayerNickName;
	struct stLocalPlayer* localPlayerData;
	int localPlayerPing;
	int localPlayerScore;
	struct stRemotePlayer* remotePlayerInfo[SAMP_MAX_PLAYERS];
	int isListed[SAMP_MAX_PLAYERS];
	uint32_t playerIp[SAMP_MAX_PLAYERS]; // always 0

	// functions
	int SetLocalPlayerName(const char* name);
	int GetSAMPPlayerIDFromGTAHandle(uint32_t GTA_Ped_ID);
	int GetActorHandleFromSAMPPlayerID(int playerId);
	const char* GetPlayerName(int iPlayerID);
	D3DCOLOR GetPlayerColor(int id);
	short GetPlayerAnimation(int iPlayerID);
	bool IsPlayerDefined(int playerId, bool check_streamed = false);
	struct stOnFootData* GetOnFootData(int playerId);
	struct stInCarData* GetInCarData(int playerId);
	struct stPassengerData* GetPassengerData(int playerId);
	struct stAimData* GetAimData(int playerId);
	struct stTrailerData* GetTrailerData(int playerId);
	bool SetPlayerColor(int id, D3DCOLOR color);
};

struct stControllerState {
	short leftStickX; // move/steer left = -128, right = 128
	short leftStickY; // move back = 128, forwards = -128
	union {
		struct {
			unsigned char leftShoulder1 : 1;  // fire weapon alt
			unsigned char shockButtonL : 1;   // crouch
			unsigned char buttonCircle : 1;   // fire weapon
			unsigned char buttonCross : 1;    // sprint, accelerate
			unsigned char buttonTriangle : 1; // enter/exit vehicle
			unsigned char buttonSquare : 1;   // jump, reverse
			unsigned char rightShoulder2 : 1; // look right (incar)
			unsigned char rightShoulder1 : 1; // hand brake, target

			unsigned char leftShoulder2 : 1; // look left
			unsigned char shockButtonR : 1;  // look behind
			unsigned char pedWalk : 1;       // walking
			unsigned char rightStickDown : 1;
			unsigned char rightStickUp : 1;
			unsigned char rightStickRight : 1; // num 4
			unsigned char rightStickLeft : 1;  // num 6
		};
		short value;
	};
};

struct stAnimation {
	union {
		struct {
			unsigned short id : 16;
			unsigned char framedelta : 8;
			unsigned char loopA : 1;
			unsigned char lockX : 1;
			unsigned char lockY : 1;
			unsigned char lockF : 1;
			unsigned char time : 2;
		};
		int value;
	};
};

struct stOnFootData {
	stControllerState controllerState;
	float position[3];
	float quaternion[4];
	uint8_t health;
	uint8_t armor;
	uint8_t currentWeapon;
	uint8_t specialAction;
	float moveSpeed[3];
	float surfingOffsets[3];
	uint16_t surfingVehicleId;
	stAnimation currentAnimation;
};

struct stInCarData {
	uint16_t vehicleId;
	stControllerState controllerState;
	float quaternion[4];
	float position[3];
	float moveSpeed[3];
	float vehicleHealth;
	uint8_t playerHealth;
	uint8_t armor;
	uint8_t currentWeapon;
	uint8_t siren;
	uint8_t landingGearState;
	uint16_t trailerId;
	union {
		uint16_t hydraThrustAngle[2]; // nearly same value
		float trainSpeed;
	};
};

struct stAimData {
	uint8_t camMode;
	float vecAimf1[3];
	float vecAimPos[3];
	float aimZ;
	uint8_t camExtZoom : 6;  // 0-63 normalized
	uint8_t weaponState : 2; // see eWeaponState
	uint8_t unk;
};

struct stTrailerData {
	uint16_t trailerId;
	float position[3];
	// float
	// fRoll[3]; float
	// fDirection[3];
	float quaternion[4]; // not tested
	float speed[3];
	float unk[2];
	uint32_t pad;
};

struct stPassengerData {
	uint16_t vehicleId;
	uint8_t seatId;
	uint8_t currentWeapon;
	uint8_t health;
	uint8_t armor;
	stControllerState controllerState;
	float position[3];
};

struct stDamageData {
	uint16_t vehicleIdLastDamageProcessed;
	int bumperDamage;
	int doorDamage;
	uint8_t lightDamage;
	uint8_t wheelDamage;
};

struct stSurfData {
	int isSurfing;
	float surfPosition[3];
	int unk0;
	uint16_t surfingVehicleId;
	uint32_t surfTick;
	struct stSAMPVehicle* surfingVehicle;
	int unk1;
	int surfMode; // 0 = not surfing, 1 = moving (unstable surf), 2 = fixed on
				  // vehicle
};

struct stUnoccupiedData {
	int16_t vehicleId;
	uint8_t seatId;
	float roll[3];
	float direction[3];
	float position[3];
	float moveSpeed[3];
	float turnSpeed[3];
	float health;
};

struct stBulletData {
	uint8_t type;
	uint16_t targetId;
	float origin[3];
	float target[3];
	float center[3];
	uint8_t weaponId;
};

struct stSpectatorData {
	stControllerState controllerState;
	float position[3];
};

struct stStatsData {
	int money;
	int ammo; // ?
};

struct stHeadSync {
	float headSync[3];
	int headSyncUpdateTick;
	int headSyncLookTick;
};

struct SAMPFUNCS_API stLocalPlayer {
	struct stSAMPPed* sampActor;
	uint16_t currentAnimId;
	uint16_t animFlags;
	uint32_t unk0;
	int isActive;
	int isWasted;
	uint16_t currentVehicleId;
	uint16_t lastVehicleId;
	struct stOnFootData onFootData;
	struct stPassengerData passengerData;
	struct stTrailerData trailerData;
	struct stInCarData inCarData;
	struct stAimData aimData;
	struct stSpawnInfo {
		uint8_t teamId;
		int skinId;
		uint8_t unk;
		float position[3];
		float rotation;
		int weapon[3];
		int ammo[3];
	} spawnInfo;
	BOOL hasSpawnInfo;
	BOOL spawnClassIsLoaded;
	uint32_t spawnSelectionTick;
	uint32_t spawnSelectionStart;
	int isSpectating;
	uint8_t teamID2;
	uint16_t unk2;
	uint32_t sendTick;
	uint32_t spectateTick;
	uint32_t aimTick;
	uint32_t statsUpdateTick;
	uint32_t weapUpdateTick;
	uint16_t aimingAtPid;
	uint16_t unk3;
	uint8_t currentWeapon;
	uint8_t weaponInventory[13];
	int weaponAmmo[13];
	int passengerDriveBy;
	uint8_t currentInterior;
	int isInRcVehicle;
	uint16_t targetObjectId;
	uint16_t targetVehicleId;
	uint16_t targetPlayerId;
	struct stHeadSync headSyncData;
	uint32_t headSyncTick;
	uint8_t space3[260];
	struct stSurfData surfData;
	struct stClassSelection {
		BOOL enableAfterDeath;
		int selectedSpawnClassId;
		BOOL waitingForRequestToSpawn;
		BOOL isActive;
	} classSelection;
	uint32_t unk4;
	uint8_t spectateMode; // 3 = vehicle, 4 = player, side = 14, fixed = 15
	uint8_t spectateType; // 0 = none, 1 = player, 2 = vehicle
	int spectateId;
	int initiatedSpectating;
	struct stDamageData vehicleDamageData;

	// functions
	void Spawn(void);
	void SendDeath(void);
	void SetSpecialAction(uint8_t byteSpecialAction);
	void RequestClass(int ClassId);
	void Say(char* msg);
	void SendEnterVehicle(int vehicleId, int passenger);
	void SendExitVehicle(int vehicleId);
	void ForceSendVehicleSync(uint16_t vehicleId);
	void ForceSendUnoccupiedSync(uint16_t vehicleId, uint8_t seatId);
	void ForceSendOnfootSync(void);
	void ForceSendAimSync(void);
	void ForceSendTrailerSync(uint16_t vehicleId);
	void ForceSendPassengerSync(uint16_t vehicleId, uint8_t seatId);
	void ForceSendStatsSync(void);
	void ForceSendWeaponsSync(void);
};

struct stRemotePlayerData {
	struct stSAMPPed* sampActor;
	struct stSAMPVehicle* sampVehicle;
	uint8_t teamId;
	uint8_t playerState;
	uint8_t seatId;
	uint32_t unk3;
	int passengerDriveBy;
	void* unk0;
	uint8_t unk1[60];
	float something[3];
	float vehicleRoll[4];
	uint32_t unk2[3];
	float onFootPos[3];
	float onFootMoveSpeed[3];
	float vehiclePosition[3];
	float vehicleMoveSpeed[3];
	uint16_t playerId;
	uint16_t vehicleId;
	uint32_t unk5;
	int showNameTag;
	int hasJetPack;
	uint8_t specialAction;
	uint32_t unk4[3];
	struct stOnFootData onFootData;
	struct stInCarData inCarData;
	struct stTrailerData trailerData;
	struct stPassengerData passengerData;
	struct stAimData aimData;
	float armor;
	float health;
	uint32_t unk10;
	uint8_t unk9;
	uint32_t tick;
	uint32_t lastStreamedInTick; // is 0 when currently streamed in
	uint32_t unk7;
	PlayerStatus status;
	struct stHeadSync headSyncData;
	int globalMarkerLoaded;
	int globalMarkerLocation[3];
	uint32_t globalMarkerGtaid;
};

struct stStreamedOutPlayerInfo {
	float position[SAMP_MAX_PLAYERS][3];
};

struct stRemotePlayer {
	stRemotePlayerData* data;
	int isNpc;
	SampString nickname;
	int score;
	int ping;
};

template<typename T>
struct stSAMPEntity {
	void* vtbl;
	uint8_t unk0[60]; // game CEntity object maybe. always empty.
	T* gtaEntity;
	uint32_t gtaEntityHandle;
};

struct stSAMPPed : public stSAMPEntity<struct actor_info> {
	int usingCellPhone;
	uint8_t unk0[600];
	struct actor_info* gtaPed;
	uint8_t unk1[22];
	uint8_t keysId;
	uint16_t gtaUrinateParticleId;
	int drinkingOrSmoking;
	int objectInHand;
	int drunkLevel;
	uint8_t unk2[5];
	int isDancing;
	int danceStyle;
	int danceMove;
	uint8_t unk3[20];
	int isUrinating;
};

struct SAMPFUNCS_API stVehiclePool {
	int vehicleCount;
	void* unk0;
	uint8_t space1[0x112C];
	struct stSAMPVehicle* sampVehicle[SAMP_MAX_VEHICLES];
	int isListed[SAMP_MAX_VEHICLES];
	struct vehicle_info* gtaVehicle[SAMP_MAX_VEHICLES];
	uint8_t space2[SAMP_MAX_VEHICLES * 6];
	uint32_t shit[SAMP_MAX_VEHICLES];
	int isListed2[SAMP_MAX_VEHICLES];
	uint32_t space3[SAMP_MAX_VEHICLES * 2];
	float spawnPos[SAMP_MAX_VEHICLES][3];
	int initiated;

	// functions
	int GetSAMPVehicleIDFromGTAHandle(uint32_t GTA_Vehicle_ID);
	int GetCarHandleFromSAMPCarID(int vehicleId);
};

struct stSAMPVehicle : public stSAMPEntity<struct vehicle_info> {
	uint32_t unk0;
	struct vehicle_info* gtaVehicle;
	uint8_t unk1[8];
	int isMotorOn;
	int isLightsOn;
	int isLocked;
	uint8_t isObjective;
	int objectiveBlipCreated;
	uint8_t unk2[16];
	uint8_t color[2];
	int colorSync;
	int colorSomething;
};

struct stObject : public stSAMPEntity<struct object_info> {
	uint8_t unk0[2];
	uint32_t unk1;
	int model;
	uint16_t unk2;
	float drawDistance;
	float unk;
	float pos[3];
	uint8_t unk3[68];
	uint8_t unk4;
	float rot[3];
	// ...
};

struct SAMPFUNCS_API stObjectPool {
	int objectCount;
	int isListed[SAMP_MAX_OBJECTS];
	struct stObject* object[SAMP_MAX_OBJECTS];

	// functions
	DWORD GetObjectHandleByID(int iID);
	int GetObjectIDByHandle(DWORD dwHandle);
	bool IsObjectExists(int iID);
};

struct stGangzone {
	float position[4];
	uint32_t color;
	uint32_t altColor;
};

struct stGangzonePool {
	struct stGangzone* gangzone[SAMP_MAX_GANGZONES];
	int isListed[SAMP_MAX_GANGZONES];
};

struct stTextLabel {
	char* text;
	uint32_t color;
	float position[3];
	float maxViewDistance;
	uint8_t showBehindWalls;
	uint16_t attachedToPlayerID;
	uint16_t attachedToVehicleID;
};

struct SAMPFUNCS_API stTextLabelPool {
	struct stTextLabel textLabel[SAMP_MAX_3DTEXTS];
	int isListed[SAMP_MAX_3DTEXTS];

	// functions
	int Create3DText(uint16_t id, char* szText, D3DCOLOR Color,
		float fPosition[3], float fViewDistance,
		bool bShowBehindWalls, USHORT sAttachedToPlayer,
		USHORT sAttachedToVehicle);
	int Create3DText(char* szText, D3DCOLOR Color, float fPosition[3],
		float fViewDistance, bool bShowBehindWalls,
		USHORT sAttachedToPlayer, USHORT sAttachedToVehicle);
	int Create3DText(uint16_t id, stTextLabel textLabel);
	bool Get3DTextInfo(uint16_t id, stTextLabel& textLabel);
	bool Destroy3DText(int ID);
};

struct stChatEntry {
	uint32_t systemTime;
	char prefix[28];
	char text[144];
	uint8_t unknown[64];
	int type; // 2 - text + prefix, 4 - text (server msg), 8 - text (debug)
	D3DCOLOR textColor;
	D3DCOLOR prefixColor; // or textOnly colour
};

struct stFontRenderer // size = 0x20
{
	ID3DXFont* chatFont;
	ID3DXFont* littleFont;
	ID3DXFont* chatShadowFont;
	ID3DXFont* littleShadowFont;
	ID3DXFont* carNumberFont;
	ID3DXSprite* tempSprite;
	IDirect3DDevice9* d3dDevice;
	char* textBuffer;
};

struct SAMPFUNCS_API stChatInfo {
	int pagesize;
	void* unk;
	int mode;
	uint8_t timestamps;
	uint32_t logFileExist;
	char logFilePathChatLog[MAX_PATH + 1];
	void* gameUi;         // CDXUTDialog
	void* editBackground; // CDXUTEditBox
	void* dxutScrollBar;
	D3DCOLOR textColor;
	D3DCOLOR infoColor;
	D3DCOLOR debugColor;
	uint32_t chatWindowBottom;
	struct stChatEntry chatEntry[100];
	stFontRenderer* fontRenderer;
	ID3DXSprite* chatTextSprite;
	ID3DXSprite* sprite;
	IDirect3DDevice9* d3dDevice;
	int renderMode; // 0 - Direct Mode (slow), 1 - Normal mode
	ID3DXRenderToSurface* d3dRenderToSurface;
	IDirect3DTexture9* texture;
	IDirect3DSurface9* surface;
	D3DDISPLAYMODE* d3dDisplayMode;
	int unk1[3];
	int unk2; // smth related to drawing in direct mode
	int redraw;
	int prevScrollBarPosition;
	int fontSizeY;
	int timestampWidth;

	// functions
	void AddChatMessage(D3DCOLOR Color, const char* Text, ...);
	void AddChatMessage(ChatMessageType Type, const char* szString,
		const char* szPrefix, D3DCOLOR TextColor,
		D3DCOLOR PrefixColor);
	void SetDisplayMode(int mode);
	void SetChatInputText(char* pstr);
	std::string GetChatInputText();
};

typedef void(__cdecl* CMDPROC)(PCHAR);
struct SAMPFUNCS_API stInputInfo {
	void* d3dDevice;
	void* dxutDialog;
	class _CDXUTEditBox* dxutEditBox;
	CMDPROC commandsProcedures[SAMP_MAX_CLIENTCMDS];
	char commandNames[SAMP_MAX_CLIENTCMDS][33];
	int commandsCount;
	int enabled;
	char inputBuffer[129];
	char recallBufffer[10][129];
	char currentBuffer[129];
	int currentRecall;
	int totalRecalls;
	CMDPROC defaultCommand;

	// functions
	void AddClientCommand(char* Command, DWORD Function);
	void EnableInput(void);
	void DisableInput(void);
	void ProcessInput();
	bool UnregisterClientCommand(char* Command);
};

struct stKillEntry {
	char killer[25];
	char victim[25];
	D3DCOLOR killerColor;
	D3DCOLOR victimColor;
	uint8_t type;
};

struct stKillInfo {
	int enabled;
	struct stKillEntry killEntry[5];
	int longestNickLength;
	int offsetX;
	int offsetY;

	ID3DXFont* d3dFont;
	ID3DXFont* weaponFont1;
	ID3DXFont* weaponFont2;
	ID3DXSprite* sprite;
	IDirect3DDevice9* d3dDevice;
	int auxFontInited;
	ID3DXFont* auxFont1;
	ID3DXFont* auxFont2;
};

struct SAMPFUNCS_API stDialogInfo {
	void* vtbl;
	int textPoxX;
	int textPoxY;
	int textSizeX;
	int textSizeY;
	int btnOffsetX;
	int btnOffsetY;
	class _CDXUTDialog* dialog;
	class _CDXUTListBox* list;
	class _CDXUTEditBox* editBox;
	int isActive;
	int type;
	uint32_t dialogID;
	char* text;
	uint32_t font;
	uint32_t font2;
	char caption[64];
	uint8_t unknown;
	int serverSide;

	// functions
	void Close(int iPressedBtn);
	void ListBoxSetIndex(int iIndex);
	int ListBoxGetIndex(void);
	int ListBoxGetItemsCount(void);
	char* ListBoxGetItemText(int id);
	wchar_t* GetInputText(void);
	void SetInputText(char* pwszText);
	void ShowDialog(uint16_t wDialogId, int iStyle, char* szCaption, char* szInfo,
		char* szButton1, char* szButton2);
	int GetScrollItem();
	void SetScrollItem(int index);
};

struct stChatPlayer {
	int created;
	char probablyTheText[256]; // text max length = 144
	uint32_t tickCreated;
	uint32_t liveLength;
	uint32_t color;
	float drawDistance;
	uint32_t unknown;
};

struct SAMPFUNCS_API stMiscInfo // sizeof = 321
{
	struct stAudio {
		int soundState; // 0 - Finished, 1 - Loaded, 2 - Playing
	};
	struct stCamera {
		class CMatrix_Padded* matrix;
	};

	stAudio* audio;
	stCamera* camera;
	stSAMPPed* localPlayerPed;
	float checkpointPos[3];
	float checkpointExtent[3];
	int checkpointsEnabled;

	uint32_t checkpointMarker;
	float raceCheckpointPos[3];
	float raceCheckpointNext[3];
	float raceCheckpointSize;
	uint8_t raceType;
	int raceCheckpointsEnabled;
	uint32_t raceCheckpointMarker;
	uint32_t raceCheckpointHandle;
	int cursorMode;
	uint32_t unk1;
	int clockEnabled;
	uint32_t unk2;
	int headMove;
	uint32_t fpsLimit;
	uint8_t unk3;
	uint8_t vehicleModels[212];

	void ToggleCursor(bool bToggle);
	void ToggleCursor(int mode, bool toggle);
	bool IsSettingsValueExists(char* szParam);
	int WriteSettingsValue(char* szParam, int iValue);
	int ReadSettingsValue(char* szParam);
};

struct SAMPFUNCS_API stScoreboardInfo {
	int enabled;
	int playersCount;
	float textOffsetX;
	float textOffsetY;
	float scalar;
	IDirect3DDevice9* directDevice;
	class _CDXUTDialog* dialog;
	class _CDXUTListBox* list;
	int offset;   // ?
	int isSorted; // ?

	// functions
	void Enable(bool enable);
};

struct stActorPool {
	int lastActorId;
	stSAMPEntity<void>* actor[SAMP_MAX_ACTORS]; // ?
	int isListed[SAMP_MAX_ACTORS];
	struct actor_info* gtaPed[SAMP_MAX_ACTORS];
	uint32_t unk0[SAMP_MAX_ACTORS];
	uint32_t unk1[SAMP_MAX_ACTORS];
};

struct stChatBubbleInfo {
	struct stChatPlayer chatBubble[SAMP_MAX_PLAYERS];
};

#pragma pack(pop)

class SAMPFUNCS_API SFSAMP {
	SFPluginInfo* m_PluginInfo;

public:
	typedef void(__stdcall* dialogCallback_t)(int dialogId, int buttonId,
		int listItem, const char* input);

	SFSAMP(SFPluginInfo* pluginInfo);
	SFPluginInfo* getPluginInfo(void);

	bool IsInitialized(void);
	DWORD getSAMPAddr(void);

	struct stSAMP* getInfo(void);
	struct stPlayerPool* getPlayers(void);
	struct stVehiclePool* getVehicles(void);
	struct stChatInfo* getChat(void);
	struct stInputInfo* getInput(void);
	struct stKillInfo* getDeathList(void);
	struct stDialogInfo* getDialog(void);
	struct stMiscInfo* getMisc(void);
	struct stScoreboardInfo* getScoreboard(void);
	struct stStreamedOutPlayerInfo* getStreamedOutInfo(void);

	bool registerChatCommand(std::string command, CommandProc callback);
	bool registerDialogCallback(dialogCallback_t callback);
	void disconnect(int Reason);
	void requestClass(int ClassId);
	void sendSCMEvent(int vehicleId, int iEvent, int iParam1, int iParam2);
	void sendEnterVehicle(int vehicleId, int passenger);
	void sendExitVehicle(int vehicleId);
	void sendSpawn(void);
	void sendWastedNotification(uint8_t reason, uint16_t playerId);
	void sendRCONCommand(char* pszCommand);
	void sendDamageVehicle(WORD vehicleID, DWORD panel, DWORD door, BYTE lights,
		BYTE tires);
	void sendClickPlayer(uint16_t playerId, uint8_t source);
	void sendDialogResponse(uint16_t dialogId, uint8_t button, uint16_t listItem,
		char* pszInput);
	void sendClickTextDraw(uint16_t textDrawId);
	void sendGiveDamage(uint16_t targetId, float damage, int weapon,
		int bodypart);
	void sendTakeDamage(uint16_t issuerId, float damage, int weapon,
		int bodypart);
	void sendEditObject(bool playerObject, uint16_t objectId, int response,
		float pos_x, float pos_y, float pos_z, float rot_x,
		float rot_y, float rot_z);
	void sendEditAttachedObject(int response, int id, int model, int bone,
		float offset_x, float offset_y, float offset_z,
		float rot_x, float rot_y, float rot_z,
		float scale_x, float scale_y, float scale_z);
	void sendInteriorChange(uint8_t interiorId);
	void sendRequestSpawn(void);
	void sendPickedUpPickup(int pickup);
	void sendMenuSelect(uint8_t row);
	void sendMenuQuit(void);
	void sendVehicleDestroyed(uint16_t vehicleId);
	void setSendRate(SendrateType eType, int iValue);
	void takeScreenShot(void);
	void setChatCommandDescription(std::string command, std::string description);
};

#pragma warning(pop)

#endif // SFSAMP_H_V__
