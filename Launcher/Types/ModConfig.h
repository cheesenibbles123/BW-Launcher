#pragma once
#include <string>
#include "AchievementConfig.h"

struct ModConfig
{
public:
	/* Also referred to as the mod ID */
	std::string ModName;
	std::string ModAuthor;
	std::string ModIconPath;
	std::string ModDllName;

	std::vector<AchievementInfo*> Achievements;
};