// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include "pch.h"
#include <mscoree.h>
#include <comdef.h>
#include <iostream>
#include <metahost.h>
#pragma comment(lib, "mscoree.lib")

void LoadAndCallAssembly()
{
    ICLRMetaHost* pMetaHost = NULL;
    ICLRRuntimeInfo* pRuntimeInfo = NULL;
    ICLRRuntimeHost* pRuntimeHost = NULL;

    // Load and start the .NET runtime
    HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
    if (FAILED(hr)) {
        std::cerr << "CLRCreateInstance failed" << std::endl;
        return;
    }

    hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
    if (FAILED(hr)) {
        std::cerr << "GetRuntime failed" << std::endl;
        return;
    }

    BOOL fLoadable;
    hr = pRuntimeInfo->IsLoadable(&fLoadable);
    if (FAILED(hr) || !fLoadable) {
        std::cerr << "Runtime is not loadable" << std::endl;
        return;
    }

    hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_PPV_ARGS(&pRuntimeHost));
    if (FAILED(hr)) {
        std::cerr << "GetInterface failed" << std::endl;
        return;
    }

    hr = pRuntimeHost->Start();
    if (FAILED(hr)) {
        std::cerr << "CLR runtime start failed" << std::endl;
        return;
    }

    // Load the C# assembly and call the method
    DWORD result;
    hr = pRuntimeHost->ExecuteInDefaultAppDomain(
        L"Path\\To\\Your\\CSharpDLL.dll",
        L"GameClientPipes.PipeController",
        L"Start",
        NULL,
        &result
    );

    if (FAILED(hr)) {
        std::cerr << "ExecuteInDefaultAppDomain failed" << std::endl;
    }

    // Clean up
    pRuntimeHost->Release();
    pRuntimeInfo->Release();
    pMetaHost->Release();
}


DWORD WINAPI MainThread(LPVOID param) {
    while (true) {
        if (GetAsyncKeyState(VK_F6) & 0x80000) {
            MessageBoxA(NULL, "F6 pressed", "F6 Pressed", MB_OK);
        }
        Sleep(100);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, L"DLL Injected!", L"Success", MB_OK | MB_ICONINFORMATION);
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

