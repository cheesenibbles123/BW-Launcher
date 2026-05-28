#pragma once
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "Logger.h"

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "AchievementManager.h"
#pragma comment (lib, "Ws2_32.lib")

 // WIP SOCKETS

class CommunicationHandler
{
public:
	CommunicationHandler(Logger* inLogger, AchievementManager* inAchievementManager);
	void Destroy();
	int SetupSocket();

private:
	Logger* logger;
	AchievementManager* achievementManager;
	SOCKET listenSocket;
	SOCKET clientSocket;
};

