#include "MainApp.h"
#include <istreamwrapper.h>
#include <chrono>
#include <thread>

std::string GetModsEnabledTxtPath()
{
    const wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    const std::string appPath(f.GetPath() + "/ModsEnabled.txt");
    return appPath;
}

ModConfig LoadConfigFromJson(std::string path, Logger* logger)
{
    const std::filesystem::path configFilePath = (path + "/config.json");
    if (!std::filesystem::exists(configFilePath))
    {
        logger->log("Missing config.json in " + configFilePath.string());
        return {};
    }

    std::ifstream ifs{ (configFilePath) };
    if (!ifs.is_open())
    {
        logger->log("Unable to open config file in " + configFilePath.string());
        return {};
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document jsonObj{};
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
        return {};
    }

    if (jsonObj.HasMember("ModAuthor") && jsonObj["ModAuthor"].IsString())
    {
        modData.ModAuthor = jsonObj["ModAuthor"].GetString();
    }
    else
    {
        logger->log("Config is missing a correct ModAuthor entry in " + configFilePath.string());
        return {};
    }

    if (jsonObj.HasMember("ModIconPath") && jsonObj["ModIconPath"].IsString())
    {
        modData.ModIconPath = jsonObj["ModIconPath"].GetString();
    }
    else
    {
        logger->log("Config is missing a correct ModIconPath entry in " + configFilePath.string());
        return {};
    }
    logger->log(std::string("Loaded mod: ") + jsonObj["ModName"].GetString());
    return modData;
}

std::vector<AchievementInfo*> LoadAchievementsFromJson(std::string path, Logger* logger)
{
    const std::filesystem::path achievementsFilePath = (path + "/achievements.json");
    logger->log("Loading achievements from " + achievementsFilePath.string());
    std::vector<AchievementInfo*> modArr = {};

    if (!std::filesystem::exists(achievementsFilePath))
    {
        logger->log("Missing achievements.json in " + achievementsFilePath.string());
        return modArr;
    }
    else {
        logger->log("Found achievements.json file");
    }

    std::ifstream ifs{ (achievementsFilePath) };
    if (!ifs.is_open())
    {
        logger->log("Unable to open achievements file in " + achievementsFilePath.string());
        return modArr;
    }
    else {
        logger->log("Opened file successfully");
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document jsonObj{};
    jsonObj.ParseStream(isw);

    if (jsonObj.HasParseError())
    {
        logger->log("Error with json format, please check the achievements file in " + achievementsFilePath.string());
    }
    auto arr = jsonObj.GetArray();
    logger->log("Fetched achievements list. Looping over each item...");

    for (int i = 0; i < arr.Size(); i++)
    {
        AchievementInfo* modData = new AchievementInfo{};
        if (arr[i].HasMember("ID") && arr[i]["ID"].IsString())
        {
            modData->ID = arr[i]["ID"].GetString();
        }
        else
        {
            logger->log("Config is missing a correct ID entry in " + achievementsFilePath.string());
            continue;
        }

        if (arr[i].HasMember("DisplayText") && arr[i]["DisplayText"].IsString())
        {
            modData->DisplayText = arr[i]["DisplayText"].GetString();
        }
        else
        {
            logger->log(modData->ID + " is missing a correct DisplayText entry in " + achievementsFilePath.string());
            continue;
        }

        if (arr[i].HasMember("type") && arr[i]["type"].IsInt())
        {
            modData->type = static_cast<AchievementType>(arr[i]["type"].GetInt());
        }
        else
        {
            logger->log(modData->ID + " is missing a correct type entry in " + achievementsFilePath.string());
            continue;
        }

        if (arr[i].HasMember("currentScore") && arr[i]["currentScore"].IsInt())
        {
            modData->currentScore = arr[i]["currentScore"].GetInt();
        }
        else
        {
            logger->log(modData->ID + " is missing a correct currentScore entry in " + achievementsFilePath.string());
            continue;
        }

        if (arr[i].HasMember("goalScore") && arr[i]["goalScore"].IsInt())
        {
            modData->goalScore = arr[i]["goalScore"].GetInt();
        }
        else
        {
            logger->log(modData->ID + " is missing a correct goalScore entry in " + achievementsFilePath.string());
            continue;
        }

        if (arr[i].HasMember("completeImageUrl") && arr[i]["completeImageUrl"].IsString())
        {
            modData->completeImageUrl = arr[i]["completeImageUrl"].GetString();
        }
        else
        {
            logger->log(modData->ID + " is missing a correct completeImageUrl entry in " + achievementsFilePath.string());
            continue;
        }

        if (arr[i].HasMember("incompleteImageUrl") && arr[i]["incompleteImageUrl"].IsString())
        {
            modData->incompleteImageUrl = arr[i]["incompleteImageUrl"].GetString();
        }
        else
        {
            logger->log(modData->ID + " is missing a correct incompleteImageUrl entry in " + achievementsFilePath.string());
            continue;
        }

        modArr.push_back(modData);
        logger->log(std::string("Loaded achievement: ") + arr[i]["ID"].GetString());
    }
    return modArr;
}

MainApp::MainApp()
{
	configManager = new ConfigManager();
    logger = new Logger("BWLauncher.log", true);
    achievementManager = new AchievementManager(logger);
    communicationHandler = new CommunicationHandler(logger, achievementManager);
    communicationHandler->SetupSocket();
}

void MainApp::Destroy()
{
    return; // Don't think about it too much, we'll deal with this later ;)
	configManager->Destroy();
    logger->Destroy();
    communicationHandler->Destroy();
}

void MainApp::LaunchGame()
{
    WriteActiveModsToFile();
    // TODO: Make all this stuff work on linux too
    system("cmd.exe /C start steam://rungameid/420290");

    std::vector<std::string> dlls = { "dllMain.dll" };
    char dllFullPath[MAX_PATH] = { 0 };
    const int MAX_COUNTER_RUNS = 300;
    int pid;
    int counter;

    // TODO: Split this up, there's no need to be refetching the processID everytime
    counter = 0;
    while (counter < MAX_COUNTER_RUNS) {
        pid = Lib::GetProcessId(L"Blackwake.exe");
        if (pid != 0) {
            break;
        }

        logger->log("Failed to get pid, trying again...");
        counter++;
        Sleep(300);
    }

    // NOTE: Injecting too soon just instant crashes the title
    // TODO: Find a better way than some arbritary delay (Likely to still have issues on slower systems)
    Sleep(10000);

    for (size_t i = 0; i < dlls.size(); i++)
    {
        counter = 0;
        GetFullPathNameA(dlls.at(i).c_str(), MAX_PATH, dllFullPath, NULL);

        while (counter < MAX_COUNTER_RUNS)
        {
            if (InjectDll(pid, dllFullPath)) {
                logger->log(dlls.at(i) + " injected successfully");
                break;
            }

            logger->log(dlls.at(i) + " injection failed, running attempt: " + std::to_string(counter));
            counter++;
            Sleep(300);
        }
    }
}

void MainApp::SetLoadStatusForMod(std::string modId, bool enabledState)
{
    enabledMods[modId] = enabledState;

    WriteActiveModsToFile();
}

bool MainApp::IsModEnabled(std::string modId)
{
    if (enabledMods.contains(modId))
    {
        return enabledMods[modId];
    }

    // Enable new mods by default, safe to assume if you are adding a new mod, its because you want to launch with it
    return true;
}

std::vector<ModConfig> MainApp::GetMods()
{
    if (loadedMods.size() < 1) {
        loadedMods = loadMods();
        LoadInitialActiveModsFromFile();
        achievementManager->SetModsReference(&loadedMods);
    }
    return loadedMods;
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
            ModConfig mod = LoadConfigFromJson(appPath + dirEntry.path().filename().string(), logger);

            if (mod.ModName != "")
            {
                std::vector<AchievementInfo*> achievements = LoadAchievementsFromJson(appPath + dirEntry.path().filename().string(), logger);
                mod.Achievements = std::move(achievements);
                enabledMods.emplace(mod.ModName, true);
                mods.push_back(std::move(mod));
            }
        }
        else {
            logger->log(dirEntry.path().string() + " is not a mod directory!");
        }
    }

    return mods;
}

// TODO: Make this work on linux
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

void MainApp::LoadInitialActiveModsFromFile()
{
    logger->log("Loading previous enabled mods");

    std::ifstream file(GetModsEnabledTxtPath());
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            const size_t splitCharIdx = line.find('=');
            std::string modId = line.substr(0, splitCharIdx);
            bool modEnabled = line.substr(splitCharIdx + 1, line.length()) == "1";
            enabledMods[modId] = modEnabled;
        }

        file.close();
    }
    else {
        logger->log("Unable to open modsEnabled txt");
    }

    logger->log("Loaded previous enabled mods");
}

void MainApp::WriteActiveModsToFile()
{
    std::string fileContent;

    const size_t numMods = loadedMods.size();
    for (size_t i = 0; i < numMods; i++) {
        fileContent += loadedMods[i].ModName + "=" + (enabledMods[loadedMods[i].ModName] ? "1" : "0") + "\n";
    }

    std::ofstream ofs(GetModsEnabledTxtPath());
    ofs << fileContent;
    ofs.close();
}
