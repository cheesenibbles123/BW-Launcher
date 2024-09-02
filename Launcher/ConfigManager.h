#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

enum configOption {
	windowHeight,
	windowWidth,
	blackwakeExeFilePath
};

class ConfigManager
{
public:
	int windowHeight = 200;
	int windowWidth = 500;
	std::string blackwakeExeFilePath = "Not Set";

	ConfigManager();

	void readConfig();
	void saveConfig();

	void Destroy();
private:

	std::unordered_map<std::string, configOption> stringToEnumMap =
	{
		{ "windowHeight", configOption::windowHeight },
		{ "windowWidth", configOption::windowWidth },
		{ "blackwakeExeFilePath", configOption::blackwakeExeFilePath }
	};
	std::string configFilePath = "configFile.cfg";
	const char commentCharacter = '#';
	const char splitCharacter = '=';
};

