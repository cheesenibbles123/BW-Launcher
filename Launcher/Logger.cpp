#include "Logger.h"

inline std::string getCurrentDateTime() {
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return std::string(buf);
};

Logger::Logger(std::string fileName, bool wipeFile)
{
    TCHAR buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring wFilePath(&buffer[0]);
    std::string f(wFilePath.begin(), wFilePath.end());
    std::string folderPath = f.substr(0, f.find_last_of("\\/"));

    logFileTarget = folderPath + '/' + fileName;
    if (wipeFile)
    {
        std::ofstream file(logFileTarget.c_str());
        file.close();
    }
    log("Starting logger");
}

void Logger::log(std::string text)
{
    fileStream = std::ofstream(logFileTarget.c_str(), std::ios_base::out | std::ios_base::app);
    fileStream << getCurrentDateTime() << '\t' << text << '\n';
    fileStream.close();
}

void Logger::Destroy()
{
    log("Closing logger");
}
