#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <filesystem>

static class Lib
{
public:
    static std::vector<std::string> SplitString(const std::string stringToSplit, const char delim = ' ') {
        std::vector<std::string> words;
        size_t start = 0;
        size_t end;

        while ((end = stringToSplit.find(delim, start)) != std::string::npos) {
            words.push_back(stringToSplit.substr(start, end - start));
            start = end + 1;
        }

        words.push_back(stringToSplit.substr(start));
        return words;
    }
    static std::vector<std::string> SplitCommunicationString(const std::string stringToSplit, const char delim = ' ', const int trimFirst = 0, const int trimlast = 0) {
        std::vector<std::string> words;
        size_t start = 0;
        size_t end;

        while ((end = stringToSplit.find(delim, start)) != std::string::npos) {
            const std::string part = stringToSplit.substr(start + trimFirst, end - start - trimlast);
            words.push_back(part);
            start = end + 1;
        }

        return words;
    }

    static int GetProcessId(const wchar_t* target) {
        DWORD pid = 0;
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        bool isClosed = false;

        do {
            if (_wcsicmp(pe32.szExeFile, target) == 0) {
                CloseHandle(hSnapshot);
                isClosed = true;
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));

        if (!isClosed) CloseHandle(hSnapshot);
        return pid;
    }

    static std::string ModsPath()
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::string appPath = cwd.generic_string() + "/Mods/";
        return appPath;
    }
};
