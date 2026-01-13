// NativeBootstrap.cpp
#include "pch.h"
#include "../Launcher/Lib.h"
#include <Psapi.h>
#include <windows.h>
#include <filesystem>
#include <string>

#define MAX_PATH 260

struct MonoDomain; struct MonoAssembly; struct MonoImage; struct MonoClass; struct MonoMethod; struct MonoThread; struct MonoString; struct MonoObject;
static MonoDomain* (*mono_get_root_domain)();
static MonoThread* (*mono_thread_attach)(MonoDomain*);
static MonoAssembly* (*mono_domain_assembly_open)(MonoDomain*, const char*);
static MonoImage* (*mono_assembly_get_image)(MonoAssembly*);
static MonoClass* (*mono_class_from_name)(MonoImage*, const char*, const char*);
static MonoMethod* (*mono_class_get_method_from_name)(MonoClass*, const char*, int);
static void* (*mono_runtime_invoke)(MonoMethod*, void*, void**, MonoObject**);
static MonoString* (*mono_string_new)(MonoDomain*, const char*);
static MonoString* (*mono_object_to_string_t)(MonoObject*, MonoObject**);
static char* (*mono_string_to_utf8)(MonoString*);

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
    R(mono_runtime_invoke); R(mono_string_new); R(mono_object_to_string_t); R(mono_string_to_utf8);
#undef R
}

static std::string ExeDir()
{
    char buf[MAX_PATH]; GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path().string();
}

static void initThreader(MonoImage* img) {
    MonoClass* threader = mono_class_from_name(img, "Tools", "MainThreadDispatcher");
    if (!threader) return;
    MonoMethod* init = mono_class_get_method_from_name(threader, "Initialize", 0);
    if (!init) return;
    mono_runtime_invoke(init, nullptr, nullptr, nullptr);
}

static void initModLoader(MonoImage* img, MonoDomain* dom, const char* path) {
    MonoClass* k = mono_class_from_name(img, "ModLoader", "Loader"); // MonoImage, namespace, class
    if (!k) return;

    MonoMethod* load = mono_class_get_method_from_name(k, "Load", 1);
    if (!load) return;

    std:: string nPath = std::string(path);
    nPath.resize(nPath.length() - 14);

    MonoString* monoName = mono_string_new(dom, nPath.c_str());
    void* args[1] = { monoName };

    MonoObject* exception = nullptr;
    mono_runtime_invoke(load, nullptr, args, &exception);
    if (exception) {
        MonoString* excStr = mono_object_to_string_t(exception, nullptr);
        char* str = mono_string_to_utf8(excStr);
        MessageBoxA(NULL, str, "Mono Exception", MB_OK);
    }
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

    initThreader(img);
    initModLoader(img, dom, outPath);

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