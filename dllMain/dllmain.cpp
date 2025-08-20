// NativeBootstrap.cpp
#include "pch.h"
#include "../Launcher/Lib.h"
#include <Psapi.h>
#include <windows.h>
#include <filesystem>
#include <string>

#define MAX_PATH 260

struct MonoDomain; struct MonoAssembly; struct MonoImage; struct MonoClass; struct MonoMethod; struct MonoThread;
static MonoDomain* (*mono_get_root_domain)();
static MonoThread* (*mono_thread_attach)(MonoDomain*);
static MonoAssembly* (*mono_domain_assembly_open)(MonoDomain*, const char*);
static MonoImage* (*mono_assembly_get_image)(MonoAssembly*);
static MonoClass* (*mono_class_from_name)(MonoImage*, const char*, const char*);
static MonoMethod* (*mono_class_get_method_from_name)(MonoClass*, const char*, int);
static void* (*mono_runtime_invoke)(MonoMethod*, void*, void**, void**);

static HMODULE WaitForMono()
{
    HMODULE h{};
    while (!(h = GetModuleHandleA("mono-2.0-bdwgc.dll")) &&
        !(h = GetModuleHandleA("mono-2.0-sgen.dll")) &&
        !(h = GetModuleHandleA("mono.dll")))
        Sleep(100);
    return h;
}

static void Resolve(HMODULE m)
{
#define R(x) x = (decltype(x))GetProcAddress(m, #x)
    R(mono_get_root_domain); R(mono_thread_attach); R(mono_domain_assembly_open);
    R(mono_assembly_get_image); R(mono_class_from_name); R(mono_class_get_method_from_name);
    R(mono_runtime_invoke);
#undef R
}

static std::string ExeDir()
{
    char buf[MAX_PATH]; GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path().string();
}

static DWORD LoadModloader(LPVOID)
{
    HMODULE mono = WaitForMono();
    Resolve(mono);

    MonoDomain* dom = mono_get_root_domain();
    mono_thread_attach(dom);

    const int pid = Lib::GetProcessId(L"BWLauncher.exe");
    std::string path;
    char outPath[MAX_PATH];
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    GetModuleFileNameExA(processHandle, NULL, outPath, sizeof(outPath));
    
    path = std::string(outPath);
    path.resize(path.length() - 14);
    path += "ModLoader.dll";

    MonoAssembly* a = mono_domain_assembly_open(dom, path.c_str());
    if (!a) return 0;

    MonoImage* img = mono_assembly_get_image(a);
    MonoClass* k = mono_class_from_name(img, "ModLoader", "Loader"); // MonoImage, namespace, class
    if (!k) return 0;
    MonoMethod* init = mono_class_get_method_from_name(k, "Load", 0);
    if (!init) return 0;

    mono_runtime_invoke(init, nullptr, nullptr, nullptr);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE h, DWORD r, LPVOID)
{
    if (r == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(h);
        CreateThread(nullptr, 0, LoadModloader, nullptr, 0, nullptr);
    }
    return TRUE;
}