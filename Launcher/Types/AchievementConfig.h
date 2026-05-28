#pragma once
#include <string>
#include <document.h>

enum AchievementType
{
	DEFAULT,
	STAGED
};

struct AchievementInfo
{
	std::string ID;
	std::string DisplayText;
	AchievementType type;
	int currentScore;
	int goalScore;
	std::string completeImageUrl;
	std::string incompleteImageUrl;

	rapidjson::Value ToJSON(rapidjson::Document::AllocatorType& allocator) const {
		rapidjson::Value obj(rapidjson::kObjectType);

		obj.AddMember("ID", rapidjson::Value(ID.c_str(), allocator), allocator);
		obj.AddMember("DisplayText", rapidjson::Value(DisplayText.c_str(), allocator), allocator);
		obj.AddMember("type", type, allocator);
		obj.AddMember("currentScore", currentScore, allocator);
		obj.AddMember("goalScore", goalScore, allocator);
		obj.AddMember("completeImageUrl", rapidjson::Value(completeImageUrl.c_str(), allocator), allocator);
		obj.AddMember("incompleteImageUrl", rapidjson::Value(incompleteImageUrl.c_str(), allocator), allocator);

		return obj;
	}
};

typedef struct AchievementUpdate
{
	std::string ID;
	std::string modID;
	int newScore = 0;
	AchievementUpdate(
		std::string inModId,
		std::string inId,
		int inNewScore
	)
	{
		modID = inModId;
		ID = inId;
		newScore = inNewScore;
	}
} AchievementUpdate;