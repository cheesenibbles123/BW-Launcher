#pragma once
#include "MainApp.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "AchievementManager.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>

#define PIPE_NAME L"\\\\.\\pipe\\MyNamedPipe"

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

static class ComLib
{
    static HANDLE fileHandle;

    static void SetupNamedPipe(MainApp* app) {
        HANDLE hPipe;
        char buffer[1024];
        DWORD bytesRead;

        // Create a named pipe
        hPipe = CreateNamedPipe(
            PIPE_NAME,             // Pipe name
            PIPE_ACCESS_DUPLEX,    // Read/Write access
            PIPE_TYPE_MESSAGE |    // Message type pipe
            PIPE_READMODE_MESSAGE |// Message-read mode
            PIPE_WAIT,             // Blocking mode
            PIPE_UNLIMITED_INSTANCES, // Max instances
            1024,                  // Output buffer size
            1024,                  // Input buffer size
            0,                     // Client time-out
            NULL);                 // Default security attribute

        if (hPipe == INVALID_HANDLE_VALUE) {
            app->logger->log("CreateNamedPipe failed, GLE=" + GetLastError());
            return;
        }
        #define PIPE_NAME L"\\\\.\\pipe\\bw-modlauncher-named-pipe-main"
        app->logger->log("Waiting for client to connect...");

        // Wait for the client to connect
        BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (!connected) {
            app->logger->log("ConnectNamedPipe failed, GLE=" + GetLastError());
            CloseHandle(hPipe);
            return;
        }

        app->logger->log("Client connected, waiting for message...");

        // Read client message
        BOOL success = ReadFile(
            hPipe,                // Handle to pipe
            buffer,               // Buffer to receive data
            sizeof(buffer),       // Size of buffer
            &bytesRead,           // Number of bytes read
            NULL);                // Not overlapped I/O

        if (!success || bytesRead == 0) {
            app->logger->log("ReadFile failed, GLE=" + GetLastError());
            CloseHandle(hPipe);
            return;
        }

        app->logger->log("Received message: " + std::string(buffer));

        // Close the pipe
        CloseHandle(hPipe);
    }

public:
    static void handleComInput(const std::string input, MainApp* app)
    {

        if (input.length() > 0)
        {
            std::vector<std::string> inputs = Lib::SplitString(input);
            const ComInputType type = ComInputMap[inputs[0]];
            switch (type)
            {
            case ComInputType::Achievement:
                recieveAchievement(inputs, app);
                break;
            default:
                app->logger->log("Got invalid ComType: " + inputs[0]);
                break;
            }
        }
    }

    static void SetupComSocket(MainApp* app)
    {
        // Clear commands
        /*TCHAR buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::wstring wFilePath(&buffer[0]);
        std::string f(wFilePath.begin(), wFilePath.end());
        std::string folderPath = f.substr(0, f.find_last_of("\\/"));

        const std::string filePath = folderPath + '/' + "comFile.txt";

        std::ifstream comConnection;
        comConnection.open(filePath, std::ios::out);
        if (comConnection.fail()) return app->logger->log("Unable to open comFile");
        comConnection.close();

        app->logger->log("Started listening for com updates");

        std::string input;
        int boolCheck;*/
        // int lastPosition = -1;;

        //while (app->isRunning)
        //{
            //comConnection.open(filePath, std::ios::in);
            //int eof = FindEndOfFile(comConnection);
            //if (eof > lastPosition)
            //{
            //    comConnection.seekg(eof, std::ios::end);
            //    comConnection >> input;

            //    // Could be multiple lines between last and new
            //    std::vector<std::string> inputs = Lib::SplitString(input);
            //    for (size_t i = 0; i < inputs.size(); i++)
            //    {
            //        handleComInput(inputs.at(i), app);
            //    }

            //    lastPosition = eof;
            //}
            /*if (std::getline(comConnection, input, '\n'))
            {
                handleComInput(input, app);
            }
            else
            {
                comConnection.open(filePath, std::ios::out | std::ios::trunc);
                comConnection.
                comConnection.close();
            }*/
            //Sleep(500);
        //}
        SetupNamedPipe(app);
    }
private:
    static void recieveAchievement(std::vector<std::string> inputs, MainApp* app)
    {
        int numPlaceholder;
        int boolCheck;

        std::istringstream(inputs[2]) >> numPlaceholder;
        std::istringstream(inputs[3]) >> boolCheck;

        AchievementUpdate info = {
            inputs[1],
            numPlaceholder,
            boolCheck == 1
        };

        app->achievementManager->AchievementUpdated(info);
    }
};