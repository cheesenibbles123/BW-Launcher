#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

enum configOption {
	windowHeight,
	windowWidth,
	serverExePath,
	isServer
};

class ConfigManager
{
public:
	int windowHeight = 200;
	int windowWidth = 500;
	bool isServer = false;
	std::string serverExePath = "";

	ConfigManager();

	void readConfig();
	void saveConfig();

	void Destroy();
private:

	std::unordered_map<std::string, configOption> stringToEnumMap =
	{
		{ "windowHeight", configOption::windowHeight },
		{ "windowWidth", configOption::windowWidth },
		{ "serverExePath", configOption::serverExePath },
		{ "isServer", configOption::isServer },
	};
	std::string configFilePath = "configFile.cfg";
	const char commentCharacter = '#';
	const char splitCharacter = '=';
};

