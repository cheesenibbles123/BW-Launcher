using System.IO;
using UnityEngine;

namespace ModLoader
{
    public static class Loader
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

                modLogger.DebugLog("Checked Assets");

                ModLoader modLoader = new ModLoader(modLogger);
                ModLoader.Instance = modLoader;
                modLoader.RefreshModFiles();

                foreach (FileInfo fileInfo in modLoader.GetAllMods().Keys)
                {
                    if (fileInfo.Name[0] != '_')
                    {
                        modLoader.Load(fileInfo);
                    }
                }

                GameObject gameObject = new GameObject();
                gameObject.AddComponent<GUI>();
                UnityEngine.Object.DontDestroyOnLoad(gameObject);
            });
        }
    }
}
