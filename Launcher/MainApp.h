#pragma once
#include "ConfigManager.h"
#include "SteamHeaders/steam_api.h"
#include "Logger.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <thread>
#include <tlhelp32.h>
#include "Lib.h"
#include "AchievementManager.h"

static enum ComInputType
{
	Achievement,
	Stat
};

static std::unordered_map<std::string, ComInputType> ComInputMap =
{
	{"Achievement", Achievement},
	{"Stat", Stat},
};

class MainApp
{
public:
	ConfigManager* configManager;
	AchievementManager* achievementManager;
	Logger* logger;
	HANDLE remoteThread;
	PROCESS_INFORMATION gameProcessInfo;
	bool isRunning;

	MainApp();

	void SetWindow(HWND newHWnd);
	void RefreshWindow();

	void Destroy();
	void LaunchGame();

private:
	HWND hWnd;

	bool InjectDll(DWORD processID, const char* dllPath);
	// void SetupSocket();
};

