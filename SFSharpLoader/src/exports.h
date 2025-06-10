#pragma once

#include "plugin.h"
#include <game_api.h>

struct CSharpExports;

typedef void(__stdcall* CSharpLoopCallback)();
typedef int(__stdcall* CSharpInitFunction)(CSharpExports*);

struct CSharpExports {
	int version = sizeof(CSharpExports);

	CSharpLoopCallback loopCallback = nullptr;

	void(__stdcall* logToChat)(const char*) = [](const char* text) {
		SF->getSAMP()->getChat()->AddChatMessage(0xAAAAAA, text);
		};

	void(__stdcall* sendToChat)(const char*) = [](const char* text) {
		SF->getSAMP()->getPlayers()->localPlayerData->Say((char*)text);
		};

	short(__stdcall* getLocalPlayerId)() = []() {
		return (short)SF->getSAMP()->getPlayers()->localPlayerId;
		};

	short(__stdcall* getAimedPlayerId)() = []() {
		return (short)SF->getSAMP()->getPlayers()->localPlayerData->weaponsData.aimedPlayerId;
		};

	bool(__stdcall* isPlayerDefined)(int) = [](int playerId) {
		return SF->getSAMP()->getPlayers()->IsPlayerDefined(playerId);
		};

	const char* (__stdcall* getPlayerName)(int) = [](int playerId) {
		return SF->getSAMP()->getPlayers()->GetPlayerName(playerId);
		};

	int(__stdcall* getPlayerScore)(int) = [](int playerId) {
		return SF->getSAMP()->getPlayers()->remotePlayerInfo[playerId]->score;
		};

	int(__stdcall* getPlayerPing)(int) = [](int playerId) {
		return SF->getSAMP()->getPlayers()->remotePlayerInfo[playerId]->ping;
		};

	void(__stdcall* showDialog)(ushort, int, char*, char*, char*, char*) = [](ushort dialogId, int dialogStyle, char* dialogCaption, char* dialogLines, char* button1, char* button2) {
		SF->getSAMP()->getDialog()->ShowDialog(dialogId, dialogStyle, dialogCaption, dialogLines, button1, button2);
		};

	void(__stdcall* registerDialogCallback)(void(__stdcall*)(int, int, int, const char*)) = [](void(__stdcall* dialogCallback)(int, int, int, const char*)) {
		SF->getSAMP()->registerDialogCallback(dialogCallback);
		};

	bool(__stdcall* isKeyDown)(char) = [](char key) {
		return SF->getGame()->isKeyDown(key);
		};

	bool(__stdcall* isKeyPressed)(char) = [](char key) {
		return SF->getGame()->isKeyPressed(key);
		};

	void(__stdcall* registerChatCommand)(char*, void(__cdecl*) (char*)) = [](char* commandName, void(__cdecl* cmdProc) (char*)) {
		SF->getSAMP()->getInput()->AddClientCommand(commandName, (uint)cmdProc);
		};

	void(__stdcall* unregisterChatCommand)(char*) = [](char* commandName) {
		SF->getSAMP()->getInput()->UnregisterClientCommand(commandName);
		};

	stChatInfo::stChatEntry* (__stdcall* getChat)() = []() {
		return SF->getSAMP()->getChat()->chatEntry;
		};

	void(__stdcall* takeScreenshot)() = []() {
		SF->getSAMP()->takeScreenShot();
		};

	bool(__stdcall* isDialogActive)(uint) = [](uint dialogId) {
		return (!SF->getSAMP()->getDialog()->serverSide) && SF->getSAMP()->getDialog()->dialogID == dialogId;
		};

	void(__stdcall* setScoreboardVisibility)() = []() {
		SF->getSAMP()->getInfo()->UpdateScoreAndPing();
		};
};

