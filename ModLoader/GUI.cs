using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using UnityEngine;

namespace ModLoader
{
    public class GUI : MonoBehaviour
    {
        private bool debugEnabled;
        private int currentScreen;

        private Vector2 scrollPosition;
        private Vector2 size;
        private Vector2 position;

        private Dictionary<FileInfo, List<Type>> allmods;

        private void Start()
        {
            currentScreen = 0;
            scrollPosition = Vector2.zero;
            debugEnabled = false;
            size = new Vector2(1000f, 1000f);
            position = new Vector2((float)(Screen.width / 2) - size.x / 2f, (float)(Screen.height / 2) - size.x / 2f);
        }

        private void RefreshMods()
        {
            allmods = ModLoader.Instance.GetAllMods();
        }

        private void Update()
        {
            if (Input.GetKeyUp("insert"))
            {
                debugEnabled = !debugEnabled;
                RefreshMods();
            }
        }

        private void OnGUI()
        {
            if (debugEnabled)
            {
                UnityEngine.GUI.ModalWindow(0, new Rect(position, size), new UnityEngine.GUI.WindowFunction(DebugWindow), "[BWML]Debug Menu");
            }
        }

        private void DebugWindow(int windowID)
        {
            UnityEngine.GUI.DragWindow(new Rect(0f, 0f, 10000f, 20f));
            currentScreen = UnityEngine.GUI.SelectionGrid(new Rect(25f, 25f, size.x - 50f, 75f), currentScreen, new string[]
            {
                "Mods",
                "Logs",
                "Debug"
            }, 3);
            switch (currentScreen)
            {
                case 0:
                    ModWindow();
                    break;
                case 1:
                    LogWindow();
                    break;
                case 2:
                    TestingWindow();
                    break;
            }
        }

        private void TestingWindow()
        {
            if (UnityEngine.GUI.Button(new Rect(0f, 100f, size.x, 25f), "Get Refrences"))
            {
                Assembly[] assemblies = AppDomain.CurrentDomain.GetAssemblies();
                foreach (Assembly assembly in assemblies)
                {
                    ModLoader.Instance.Logger.Log(assembly.FullName);
                }
            }

            if (UnityEngine.GUI.Button(new Rect(0f, 125f, size.x, 25f), "List all mods"))
            {
                ModLoader.Instance.Logger.Log("Loaded Mods:");
                foreach (FileInfo fileInfo in allmods.Keys)
                {
                    if (ModLoader.Instance.IsLoaded(fileInfo))
                    {
                        ModLoader.Instance.Logger.Log(fileInfo.Name);
                    }
                }

                ModLoader.Instance.Logger.Log("Unloaded Mods:");
                foreach (FileInfo fileInfo2 in allmods.Keys)
                {
                    bool flag4 = !ModLoader.Instance.IsLoaded(fileInfo2);
                    if (flag4)
                    {
                        ModLoader.Instance.Logger.Log(fileInfo2.Name);
                    }
                }
            }
        }

        private void LogWindow()
        {
            int num = 0;
            if (Logger.Logs.Any<string>())
            {
                scrollPosition = UnityEngine.GUI.BeginScrollView(new Rect(0f, 100f, size.x, size.y - 100f), scrollPosition, new Rect(0f, 0f, size.x, (float)(25 * Logger.Logs.Count)));
                if (UnityEngine.GUI.Button(new Rect(0f, 0f, size.x, 25f), "Clear Logs"))
                {
                    Logger.Logs.Clear();
                }

                foreach (string text in Logger.Logs)
                {
                    num++;
                    UnityEngine.GUI.Label(new Rect(0f, (float)(25 * num), 1000f, 25f), text);
                }

                UnityEngine.GUI.EndScrollView();
            }
        }

        private void ModWindow()
        {
            scrollPosition = UnityEngine.GUI.BeginScrollView(new Rect(0f, 100f, size.x, size.y - 100f), scrollPosition, new Rect(0f, 0f, size.x, 50f));
            if (UnityEngine.GUI.Button(new Rect(0f, 0f, size.x, 25f), "Refresh all mods"))
            {
                ModLoader.Instance.RefreshModFiles();
                RefreshMods();
            }

            int num = 0;
            foreach (FileInfo fileInfo in allmods.Keys)
            {
                num++;
                if (UnityEngine.GUI.Toggle(new Rect(5f, (float)(num * 25), 150f, 25f), ModLoader.Instance.IsLoaded(fileInfo), fileInfo.Name) && !ModLoader.Instance.IsLoaded(fileInfo))
                {
                    ModLoader.Instance.Load(fileInfo);
                    RefreshMods();
                }
                else if (ModLoader.Instance.IsLoaded(fileInfo))
                {
                    ModLoader.Instance.Unload(fileInfo);
                    RefreshMods();
                }

                if (UnityEngine.GUI.Button(new Rect(155f, (float)(num * 25), 100f, 25f), "Reload"))
                {
                    ModLoader.Instance.RefreshModFiles();
                    RefreshMods();
                }

                RefreshMods();

                if (UnityEngine.GUI.Button(new Rect(255f, (float)(num * 25), 100f, 25f), "Menu"))
                {
                    foreach (GameObject gameObject in ModLoader.Instance.ModObjects[fileInfo])
                    {
                        gameObject.BroadcastMessage("OnSettingsMenu");
                    }
                }
            }

            UnityEngine.GUI.EndScrollView();
        }
    }
}
