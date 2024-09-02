#include "MainApp.h"
#include "comLib.h"

MainApp::MainApp()
{
    //if (SteamAPI_RestartAppIfNecessary(480)) // Replace with your App ID
    //{
    //    //return 1;
    //}
    isRunning = false;
	configManager = new ConfigManager();
    logger = new Logger("BWLauncher.log", true);
    achievementManager = new AchievementManager(logger);
}

void MainApp::SetWindow(HWND newHWnd)
{
    hWnd = newHWnd;
}

void MainApp::RefreshWindow()
{
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}

void MainApp::Destroy()
{
	configManager->Destroy();
    logger->Destroy();
    isRunning = false;
}

void MainApp::LaunchGame()
{
    if (!SteamAPI_Init())
    {
        //return logger->log("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
    }

    system("cmd.exe /C start steam://rungameid/420290");

    std::vector<std::string> dlls = {"dllInject.dll", "GameClientPipes.dll"};
    char dllFullPath[MAX_PATH] = { 0 };

    for (size_t i = 0; i < dlls.size(); i++)
    {
        bool gotProcessId = false;
        int counter = 0;
        GetFullPathNameA(dlls.at(i).c_str(), MAX_PATH, dllFullPath, NULL);

        while (!gotProcessId && counter < 100)
        {
            Sleep(100);
            if (InjectDll(Lib::GetProcessId(L"Blackwake.exe"), dllFullPath)) {
                logger->log(dlls.at(i) + " injected successfully");
                gotProcessId = true;
                isRunning = true;
            }
            else {
                counter++;
                logger->log(dlls.at(i) + " injection failed, running attempt: " + std::to_string(counter));
            }
        }

    }

    if (isRunning)
    {
        std::thread t(ComLib::SetupComSocket, this);
        t.detach();
    }
}

bool MainApp::InjectDll(DWORD processID, const char* dllPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess == NULL) {
        logger->log("ERROR: Failed to open process");
        return false;
    }

    LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pDllPath == NULL) {
        logger->log("ERROR: Failed to allocate memory in target process");
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        logger->log("ERROR: Failed to write DLL path to target process memory");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pDllPath, 0, NULL);
    if (hThread == NULL) {
        logger->log("ERROR: Failed to create remote thread in target process");
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return true;
}
