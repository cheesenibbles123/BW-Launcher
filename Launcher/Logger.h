#pragma once
#include <windows.h>
#include <string>
#include <time.h>
#include <fstream>
#include <iostream>

// TODO: Make this work when threaded (100% overkill, but would be nice to not freeze up the launcher when alot of data comes in)
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

