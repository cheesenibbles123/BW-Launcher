using System.IO;
using System.Linq;
using UnityEngine;

namespace ModLoader
{
    internal static class Loader
    {
        private static string[] modsToLoad = null;

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
                    if (fileInfo.Name[0] != '_' && modsToLoad != null ? modsToLoad.Contains(fileInfo.Name) : true)
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

        internal static void setModsToLoad(string[] mods)
        {
            modsToLoad = mods;
        }
    }
}
