#include "MainApp.h"
#include <istreamwrapper.h>

MainApp::MainApp()
{
    isRunning = false;
	configManager = new ConfigManager();
    logger = new Logger("BWLauncher.log", true);
    communicationHandler = new CommunicationHandler(logger);
    communicationHandler->SetupSocket(communicationHandler);
    achievementManager = new AchievementManager(logger);
}

void MainApp::Destroy()
{
    return; // Don't think about it too much ;)
	configManager->Destroy();
    logger->Destroy();
    isRunning = false;
    communicationHandler->Destroy();
}

void MainApp::LaunchGame()
{
    if (!SteamAPI_Init())
    {
        // return logger->log("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
    }

    system("cmd.exe /C start steam://rungameid/420290");

    std::vector<std::string> dlls = {"dllMain.dll"};
    char dllFullPath[MAX_PATH] = { 0 };

    for (size_t i = 0; i < dlls.size(); i++)
    {
        bool gotProcessId = false;
        int counter = 0;
        GetFullPathNameA(dlls.at(i).c_str(), MAX_PATH, dllFullPath, NULL);

        while (!gotProcessId && counter < 300)
        {
            Sleep(300);

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

    /*if (isRunning)
    {
        std::thread t(ComLib::SetupComSocket, this);
        t.detach();
    }*/
}

std::vector<ModConfig> MainApp::loadMods()
{
    std::vector<ModConfig> mods = std::vector<ModConfig>();
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    const std::string appPath(f.GetPath() + "/Mods/");
    logger->log(appPath);
    const std::filesystem::path modsFolderFilePath = appPath;
    if (!std::filesystem::exists(modsFolderFilePath))
    {
        std::filesystem::create_directory(modsFolderFilePath);
    }
    modsFolderFilePath / "/";

    rapidjson::Document jsonObj{};

    for (const std::filesystem::directory_entry& dirEntry : std::filesystem::recursive_directory_iterator(modsFolderFilePath))
    {
        if (dirEntry.is_directory())
        {
            const std::filesystem::path configFilePath = (appPath + dirEntry.path().filename().string() + "/config.json");
            if (!std::filesystem::exists(configFilePath))
            {
                logger->log("Missing config.json in " + configFilePath.string());
                continue;
            }

            std::ifstream ifs{ (configFilePath) };
            if (!ifs.is_open())
            {
                logger->log("Unable to open config file in " + configFilePath.string());
                continue;
            }

            rapidjson::IStreamWrapper isw(ifs);
            jsonObj.ParseStream(isw);

            ModConfig modData = {};
            if (jsonObj.HasParseError())
            {
                logger->log("Error with json format, please check the config file in " + configFilePath.string());
            }

            if (jsonObj.HasMember("ModName") && jsonObj["ModName"].IsString())
            {
                modData.ModName = jsonObj["ModName"].GetString();
            }
            else
            {
                logger->log("Config is missing a correct ModName entry in " + configFilePath.string());
                continue;
            }

            if (jsonObj.HasMember("ModAuthor") && jsonObj["ModAuthor"].IsString())
            {
                modData.ModAuthor = jsonObj["ModAuthor"].GetString();
            }
            else
            {
                logger->log("Config is missing a correct ModAuthor entry in " + configFilePath.string());
                continue;
            }

            if (jsonObj.HasMember("ModIconPath") && jsonObj["ModIconPath"].IsString())
            {
                modData.ModIconPath = jsonObj["ModIconPath"].GetString();
            }
            else
            {
                logger->log("Config is missing a correct ModIconPath entry in " + configFilePath.string());
                continue;
            }
            logger->log(std::string("Loaded mod: ") + jsonObj["ModName"].GetString());
            mods.push_back(std::move(modData));
        }
        else {
            logger->log(dirEntry.path().string() + " is not a mod directory!");
        }
    }

    return mods;
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
