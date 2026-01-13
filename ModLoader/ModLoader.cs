using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using UnityEngine;

namespace ModLoader
{
    public class ModLoader
    {
        public static ModLoader Instance { get; internal set; }

        private static string dllpath = "";

        public Logger Logger;

        private readonly Dictionary<FileInfo, List<Type>> allMods = new Dictionary<FileInfo, List<Type>>();

        public Dictionary<FileInfo, List<GameObject>> ModObjects = new Dictionary<FileInfo, List<GameObject>>();

        public static string FolderPath
        {
            get
            {
                return dllpath ?? Path.GetDirectoryName(dllpath);
            }
        }

        public static string ModsPath
        {
            get
            {
                return FolderPath + "\\Mods";
            }
        }

        public static string AssetsPath
        {
            get
            {
                return ModsPath + "\\Assets";
            }
        }

        public static string LogPath
        {
            get
            {
                return ModsPath + "\\Logs";
            }
        }

        /*
         * INTERNAL ONLY
         */
        public static void SetPath(string path)
        {
            dllpath = path;
        }
        public Dictionary<FileInfo, List<Type>> GetAllMods()
        {
            return new Dictionary<FileInfo, List<Type>>(allMods);
        }

        public ModLoader(Logger logger)
        {
            Logger = logger;
            CommunicationPipes.init();
        }

        public bool IsLoaded(FileInfo mod)
        {
            return ModObjects.ContainsKey(mod);
        }

        public void RefreshModFiles()
        {
            foreach (KeyValuePair<FileInfo, List<Type>> keyValuePair in GetAllMods())
            {
                RemoveModFile(keyValuePair.Key);
            }

            foreach (string modFolder in Directory.GetDirectories(ModsPath))
            {
                DirectoryInfo directoryInfo = new DirectoryInfo(modFolder + "\\dlls");
                if (directoryInfo.Exists)
                {
                    foreach (FileInfo fileInfo in directoryInfo.GetFiles("*.dll"))
                    {
                        if (GetModTypes(fileInfo).Count > 0)
                        {
                            AddModFile(fileInfo);
                        }
                    }

                }
            }
        }

        public void AddModFile(FileInfo file)
        {
            allMods.Add(file, GetModTypes(file));
        }

        public void RemoveModFile(FileInfo file)
        {
            Unload(file);
            if (allMods.ContainsKey(file))
            {
                allMods.Remove(file);
            }
        }

        private List<Type> GetModTypes(FileInfo file)
        {
            List<Type> list = new List<Type>();
            if (file == null)
            {
                Logger.Log("Got null file");
                return list;
            }

            Assembly assemblyFile = Assembly.LoadFrom(file.FullName);
            if (assemblyFile == null)
            {
                Logger.Log(file.FullName + " is not a valid assembly file");
                return list;
            }

            try
            {
                foreach (Type type in assemblyFile.GetTypes())
                {
                    if (type.GetCustomAttributes(typeof(Mod), true).Length != 0)
                    {
                        if (type.IsClass && typeof(MonoBehaviour).IsAssignableFrom(type) && !type.IsAbstract && type.IsPublic)
                        {
                            list.Add(type);
                        }
                        else
                        {
                            Logger.Log(type.FullName + ":");
                            Logger.Log("Is class: " + type.IsClass.ToString());
                            Logger.Log("Is Assignable: " + typeof(MonoBehaviour).IsAssignableFrom(type).ToString());
                            Logger.Log("Is not Abstract: " + (!type.IsAbstract).ToString());
                            Logger.Log("Is public: " + type.IsPublic.ToString());
                        }
                    }
                    else if (!type.FullName.EndsWith("Patch"))
                    {
                        Logger.Log(type.FullName + " Does not have attribute [Mod]");
                    }
                }
            } catch(Exception e)
            {
                Logger.Log("Error getting types" + e.ToString() + " in file:" + file.FullName);
            }

            return list;
        }

        public void Load(FileInfo file)
        {
            List<Type> list = new List<Type>();
            if (!IsLoaded(file) && allMods.TryGetValue(file, out list))
            {
                ModObjects.Add(file, new List<GameObject>());
                foreach (Type type in list)
                {
                    GameObject gameObject = new GameObject(type.Name);
                    gameObject.AddComponent(type);
                    UnityEngine.Object.DontDestroyOnLoad(gameObject);
                    ModObjects[file].Add(gameObject);
                    Logger.Log("Loaded: " + type.Name + " From file: " + file.Name);
                }
            }
        }

        public void Unload(FileInfo file)
        {
            List<Type> list;
            if (allMods.TryGetValue(file, out list))
            {
                if (IsLoaded(file))
                {
                    foreach (GameObject obj in ModObjects[file])
                    {
                        UnityEngine.Object.Destroy(obj);
                    }

                    ModObjects.Remove(file);
                    Logger.Log("Unloaded: " + file.Name);
                }
            }
        }
    }
}
