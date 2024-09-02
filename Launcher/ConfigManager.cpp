#include "ConfigManager.h"

ConfigManager::ConfigManager()
{
	readConfig();
}

void ConfigManager::readConfig()
{
	struct stat buffer;
	if (stat(configFilePath.c_str(), &buffer) != 0) {
		std::ofstream outfile(configFilePath);
		outfile.close();
	}

	std::ifstream file(configFilePath);
	std::string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			// Entry here
			if (line.at(0) != commentCharacter) {
				int splitCharPos = line.find(splitCharacter);

				if(splitCharPos != std::string::npos) {
					const std::string property = line.substr(0, splitCharPos);
					const std::string value = line.substr(splitCharPos + 1, line.size());

					switch (stringToEnumMap[property]) {
					case configOption::windowWidth:
						windowWidth = std::stoi(value);
						break;
					case configOption::windowHeight:
						windowHeight = std::stoi(value);
						break;
					case configOption::blackwakeExeFilePath:
						blackwakeExeFilePath = value;
						break;
					default:
						break;
					}
				}
			}
		}
		file.close();
	}
}

void ConfigManager::saveConfig()
{
}

void ConfigManager::Destroy()
{
	saveConfig();
}
