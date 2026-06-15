using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;

namespace ModLoader
{
    internal static class Loader
    {
        public static void Load(string path)
        {
            Tools.MainThreadDispatcher.RunOnMainThread(() =>
            {
                ModLoader.SetPath(path);
                string file = ModLoader.LogPath + "\\ModLoader.log";
                Logger modLogger = new Logger("[ML]", file);
                modLogger.ClearLog();
                modLogger.DebugLog("Using Filepath: " + path);

                if (!Directory.Exists(ModLoader.AssetsPath))
                {
                    Directory.CreateDirectory(ModLoader.AssetsPath);
                }

                string targetPath = Application.dataPath + "/Managed/0Harmony.dll";
                if (!File.Exists(targetPath))
                {
                    string harmonyDll = ModLoader.FolderPath + "0Harmony.dll";
                    File.Copy(harmonyDll, targetPath);
                }

                modLogger.DebugLog("Checked Assets");

                ModLoader modLoader = new ModLoader(modLogger);
                ModLoader.Instance = modLoader;
                modLoader.RefreshModFiles();

                List<string> modsToLoad;
                try
                {
                    modsToLoad = GetModsToLoad(modLogger);
                    modLogger.DebugLog("Got mods" + modsToLoad.ToString());
                }
                catch
                {
                    modsToLoad = new List<string>();
                    modLogger.DebugLog("Error loading enabled mods txt");
                }

                foreach (FileInfo fileInfo in modLoader.GetAllMods().Keys)
                {
                    modLogger.DebugLog("Checking mod: " + fileInfo.Name + " (" + fileInfo.Name.Split('.')[0] + ")");

                    if (fileInfo.Name[0] != '_' && modsToLoad.Contains(fileInfo.Name.Split('.')[0]))
                    {
                        modLoader.Load(fileInfo);
                    } else
                    {
                        modLogger.DebugLog("Skipping mod " + fileInfo.Name + " as it is not within the array of mods to load");
                    }
                }

                GameObject gameObject = new GameObject();
                gameObject.AddComponent<GUI>();
                UnityEngine.Object.DontDestroyOnLoad(gameObject);
            });
        }

        static List<string> GetModsToLoad(Logger modLogger)
        {
            List<string> modsToLoad = new List<string>();

            string file = ModLoader.FolderPath + "/ModsEnabled.txt";
            if (!File.Exists(file))
            {
                Debug.Log("Error loading config path at " + file);
            }

            string[] allLines = File.ReadAllLines(file);
            char splitCharacter = '=';

            for (int i = 0; i < allLines.Length; i++)
            {
                if (allLines[i].Contains("="))
                {
                    string[] line = allLines[i].Split(splitCharacter);
                    modLogger.DebugLog("Checking: " + line[0] + "-" + line[1] + " (" + line.Length + ")");
                    modLogger.DebugLog("isEnabled: " + (line[1] == "1").ToString());

                    if (line.Length == 2)
                    {
                        if (line[1] == "1")
                        {
                            modsToLoad.Add(line[0]);
                        }
                    }
                }
            }

            Debug.Log("Mods loaded: " + modsToLoad.Count);
            return modsToLoad;
        }
    }
}
