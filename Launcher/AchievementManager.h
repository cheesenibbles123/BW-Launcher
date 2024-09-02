#pragma once
#include "Lib.h"
#include <string>
#include <vector>
#include "Logger.h"
#include <iostream>
#include <fstream>

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

typedef struct AchievementUpdate
{
	std::string ID {};
	int newScore = 0;
	bool setComplete = false;
	AchievementUpdate(std::string newId, int newNewScore, bool newSetComplete)
	{
		ID = newId;
		newScore = newNewScore;
		setComplete = newSetComplete;
	}
} AchievementUpdate;

class AchievementManager
{
public:
	AchievementManager(Logger* parentLogger);
	void AchievementUpdated(const AchievementUpdate achievement);
private:
	Logger* logger;
	std::vector<AchievementInfo> achievements;

	void LoadAchievementsForMods();
};

