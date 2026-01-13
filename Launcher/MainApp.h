#pragma once
#include "ConfigManager.h"
#include "AchievementManager.h"
#include "CommunicationHandler.h"
#include "Logger.h"
#include "Lib.h"
#include "SteamHeaders/steam_api.h"
#include <stdio.h>
#include <tchar.h>
#include <thread>
#include <vector>
#include "Types/ModConfig.h"
#include <filesystem>
#include <document.h>
#include "writer.h"
#include "stringbuffer.h"
#include <filereadstream.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

class MainApp
{
public:
	ConfigManager* configManager;
	AchievementManager* achievementManager;
	CommunicationHandler* communicationHandler;
	Logger* logger;
	HANDLE remoteThread;
	PROCESS_INFORMATION gameProcessInfo;
	bool isRunning;

	MainApp();

	void Destroy();
	void LaunchGame();

	std::vector<ModConfig> loadMods();
	std::thread communicationThread;
private:
	HWND hWnd;

	bool InjectDll(DWORD processID, const char* dllPath);
	// void SetupSocket();
};

