#pragma once
#include "Lib.h"
#include <string>
#include <vector>
#include "Logger.h"
#include <iostream>
#include <fstream>
#include "Types/ModConfig.h"
#include "Types/AchievementConfig.h"

class AchievementManager
{
public:
	AchievementManager(Logger* parentLogger);
	void AchievementUpdated(const AchievementUpdate achievement);
	void SetModsReference(const std::vector<ModConfig>* mods);
	void Test();
private:
	void SaveAchievementsToDisk(const std::string modID, const ModConfig* config);
	Logger* logger;
	const std::vector<ModConfig>* mods;
};

