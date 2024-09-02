#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>

static class Lib
{
public:
    static std::vector<std::string> SplitString(std::string stringToSplit, char delim = ' ') {
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

    static int GetProcessId(const wchar_t* target) {
        DWORD pid = 0;
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        bool isClosed = false;
        do {
            if (wcscmp(pe32.szExeFile, target) == 0) {
                CloseHandle(hSnapshot);
                isClosed = true;
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
        if (!isClosed) CloseHandle(hSnapshot);
        return pid;
    }
};
