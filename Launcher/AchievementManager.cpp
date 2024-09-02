#include "AchievementManager.h"

AchievementManager::AchievementManager(Logger* parentLogger)
{
	logger = parentLogger;
}

void AchievementManager::AchievementUpdated(const AchievementUpdate achievement)
{
	logger->log("Achievement Updated!!!");
}

void AchievementManager::LoadAchievementsForMods()
{

}
