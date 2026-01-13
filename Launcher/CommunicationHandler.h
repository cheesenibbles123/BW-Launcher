#pragma once
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include "Logger.h"

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

 // WIP SOCKETS

class CommunicationHandler
{
public:
	CommunicationHandler(Logger* logger);
	void Destroy();
	int SetupSocket(CommunicationHandler* handler);

private:
	Logger* logger;
	SOCKET listenSocket;
	SOCKET clientSocket;
};

