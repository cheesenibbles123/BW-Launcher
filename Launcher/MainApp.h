#pragma once
#include "ConfigManager.h"
#include "AchievementManager.h"
#include "Logger.h"
#include "Lib.h"
#include "SteamHeaders/steam_api.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <thread>

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

