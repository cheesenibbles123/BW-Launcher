#pragma once
#include "Lib.h"
#include <string>
#include <vector>

static enum AchievementType
{
	DEFAULT,
	STAGED
};

struct AchievementInfo
{
	std::string ID;
	AchievementType type;
	int currentScore;
	int goalScore;
	bool isComplete;

	std::string completeImageUrl;
	std::string incompleteImageUrl;
};

struct AchievementUpdate
{
	std::string ID;
	int newScore;
	bool setComplete;
};

class AchievementManager
{
public:
	AchievementManager();
	void AchievementUpdated(AchievementUpdate achievement);
private:
	std::vector<AchievementInfo> achievements;

	void LoadAchievementsForMod();
};

