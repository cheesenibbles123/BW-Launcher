#pragma once
#include <windows.h>
#include <string>
#include <time.h>
#include <fstream>
#include <iostream>

class Logger
{
public:
	Logger(std::string fileName, bool wipeFile = false);

	void log(std::string text);
	void Destroy();

private:
	std::string logFileTarget;
	std::ofstream fileStream;
};

