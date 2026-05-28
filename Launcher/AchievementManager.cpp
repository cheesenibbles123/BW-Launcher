#include "AchievementManager.h"
#include <document.h>
#include "writer.h"
#include <stringbuffer.h>
#include <fstream>
#include <prettywriter.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

AchievementManager::AchievementManager(Logger* parentLogger)
{
	logger = parentLogger;
}

void AchievementManager::AchievementUpdated(const AchievementUpdate achievementUpdate)
{
	logger->log("Achievement Request Recieved: " + achievementUpdate.modID + ", " + achievementUpdate.ID + ", " + std::to_string(achievementUpdate.newScore));

	const size_t size = mods->size();
	bool foundAchievement = false;
	const ModConfig* config;

	for (size_t i = 0; i < size; i++) {
		config = &mods->at(i);
		if (config->ModName == achievementUpdate.modID) {
			const size_t numAchieves = config->Achievements.size();
		
			for (size_t s = 0; s < numAchieves; s++) {
				if (config->Achievements.at(s)->ID == achievementUpdate.ID) {
					AchievementInfo* achievement = config->Achievements.at(s);
					achievement->currentScore += achievementUpdate.newScore;
					foundAchievement = true;
					break;
				}
			}

			if (foundAchievement) {
				break;
			}
		}
	}

	if (foundAchievement) {
		SaveAchievementsToDisk(achievementUpdate.modID, config);
	}
}

void AchievementManager::SetModsReference(const std::vector<ModConfig>* inMods)
{
	mods = inMods;
}

void AchievementManager::Test()
{
	logger->log("test");
}

void AchievementManager::SaveAchievementsToDisk(std::string modID, const ModConfig* config)
{
	rapidjson::Document jsonObj;
	jsonObj.SetArray();

	rapidjson::Document::AllocatorType& allocator = jsonObj.GetAllocator();

	const size_t numAchievements = config->Achievements.size();
	for (size_t i = 0; i < numAchievements; i++) {
		jsonObj.PushBack(config->Achievements.at(i)->ToJSON(allocator), allocator);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	jsonObj.Accept(writer);

	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	const std::string appPath(f.GetPath() + "/Mods/");
	const std::string modFolder = appPath + modID;

	struct stat info;

	if (stat(modFolder.c_str(), &info) != 0) {
		logger->log("Cannot access: " + modFolder);
	}
	else if (info.st_mode & S_IFDIR) {
		const std::filesystem::path achievementsFilePath = (modFolder + "/achievements.json");
		std::ofstream file(achievementsFilePath);

		file << buffer.GetString();
		file.close();

		logger->log("Updated achievements file: " + modFolder + "/achievements.json");
	}
	else {
		logger->log("Does not exist: " + modFolder);
	}
}
