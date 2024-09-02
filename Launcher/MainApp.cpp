#include "MainApp.h"

MainApp::MainApp()
{
    //if (SteamAPI_RestartAppIfNecessary(480)) // Replace with your App ID
    //{
    //    //return 1;
    //}
    isRunning = false;
	configManager = new ConfigManager();
    logger = new Logger("BWLauncher.log", true);
    achievementManager = new AchievementManager();
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

int getProcId(const wchar_t* target) {
    DWORD pid = 0;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool isClosed = false;
    do {
        if (wcscmp(pe32.szExeFile, target) == 0) {
            CloseHandle(hSnapshot);
            isClosed = true;
            pid = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &pe32));
    if (!isClosed) CloseHandle(hSnapshot);
    return pid;
}

static void SetupSocket(MainApp* app)
{
    // Clear commands
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring wFilePath(&buffer[0]);
    std::string f(wFilePath.begin(), wFilePath.end());
    std::string folderPath = f.substr(0, f.find_last_of("\\/"));

    const std::string filePath = folderPath + '/' + "comFile.txt";

    std::fstream comConnection;
    comConnection.open(filePath);
    comConnection.close();

    comConnection.open(filePath, std::ios::in);
    app->logger->log(filePath);
    if (comConnection.fail()) return app->logger->log("Unable to open comFile");
    app->logger->log("Started listening for com updates");

    std::string input;

    while (app->isRunning)
    {
        //app->logger->log("starting sweep");
        std::getline(comConnection, input, '\n');
        //app->logger->log("got line" + input);
        if (input.length() > 0)
        {
            //app->logger->log("has length");
            std::vector<std::string> inputs = Lib::SplitString(input);
            const ComInputType type = ComInputMap[inputs[0]];
            switch (type)
            {
            case ComInputType::Achievement:
            {
                AchievementUpdate info;
                info.ID = inputs[1];
                info.newScore = std::stoi(inputs[2]);
                info.setComplete = std::stoi(inputs[3]) == 1;

                app->achievementManager->AchievementUpdated(info);
                //app->logger->log("Got update for: " + inputs[1]);
                break;
            }
            default:
                //app->logger->log("Got invalid ComType: " + inputs[0]);
                break;
            }
        }
        //app->logger->log("Sleeping");
        Sleep(100);
    }
}

void MainApp::LaunchGame()
{
    if (!SteamAPI_Init())
    {
        //return logger->log("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
    }

    system("cmd.exe /C start steam://rungameid/420290");

    char dll[] = "dllInject.dll";
    char dllFullPath[MAX_PATH] = { 0 };
    GetFullPathNameA(dll, MAX_PATH, dllFullPath, NULL);
    bool gotProcessId = false;
    int counter = 0;

    while (!gotProcessId && counter < 100)
    {
        if (InjectDll(getProcId(L"Blackwake.exe"), dllFullPath)) {
            logger->log("DLL injected successfully");
            gotProcessId = true;
            isRunning = true;

            std::thread t(SetupSocket, this);
            break;
        }
        else {
            counter++;
            logger->log("DLL injection failed, running attempt: " + std::to_string(counter));
        }
        Sleep(100);
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
