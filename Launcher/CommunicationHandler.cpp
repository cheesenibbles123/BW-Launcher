#include "CommunicationHandler.h"
#include <thread>

#define DEFAULT_BUFLEN 512
constexpr auto DEFAULT_PORT = "27015";

void awaitResponse(CommunicationHandler* handler, SOCKET clientSocket, Logger* logger)
{
    char buffer[4096];
    while (true)
    {
        ZeroMemory(buffer, 4096);

        int bytesReceived = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Error in recv\n";
            break;
        }

        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }

        std::string message = std::string(buffer, bytesReceived);
        logger->log("Received: " + std::string(buffer, bytesReceived));

        int openBracket = message.find("[");
        int closeBracket = message.find("]");
        std::string target = message.substr(openBracket, closeBracket);
    }
}

void awaitClient(CommunicationHandler* handler, SOCKET listenSocket, Logger* logger)
{
    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to connect to client, Error #" << WSAGetLastError() << "\n";
        logger->log("Failed to connect to client, Error #" + std::to_string(WSAGetLastError()));
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    logger->log("Connected to client socket");
    closesocket(listenSocket);

    std::thread thread(&awaitResponse, handler, clientSocket, logger);
    thread.detach();
}

CommunicationHandler::CommunicationHandler(Logger* inLogger)
{
    logger = inLogger;
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0)
    {
        std::cerr << "Can't start Winsock, Error #" << wsOK << "\n";
    }
}

void CommunicationHandler::Destroy()
{
    closesocket(listenSocket);
    closesocket(clientSocket);
}

int CommunicationHandler::SetupSocket(CommunicationHandler* handler)
{
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket, Error #" << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }
    logger->log("Initialising listen socket");

    // Binding to IP:Port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&hint, sizeof(hint)))
    {
        std::cerr << "Failed to bind socket, Error #" << WSAGetLastError() << "\n";
        logger->log("Failed to bind socket, Error #" + std::to_string(WSAGetLastError()));
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen on socket, Error #" << WSAGetLastError() << "\n";
        logger->log("Failed to listen on socket, Error #" + std::to_string(WSAGetLastError()));
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    logger->log("Finished setting up listen socket");

    std::thread thread(&awaitClient, handler, listenSocket, logger);
    thread.detach();

    return 0;
}