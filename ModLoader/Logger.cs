using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ModLoader
{
    public class Logger
    {
        private string prefix;
        private string file;

        public static List<string> Logs { get; } = new List<string>();

        public Logger(string prefix, string file = null)
        {
            this.prefix = prefix;
            this.file = file;
            if (!string.IsNullOrEmpty(file) && !File.Exists(file))
            {
                Directory.CreateDirectory(Path.GetDirectoryName(file));
                File.WriteAllText(file, "");
            }
        }

        public void ClearLog()
        {
            Logs.Clear();

            if (!string.IsNullOrEmpty(file) && File.Exists(file))
            {
                File.Delete(file);
            }
        }

        public void Log(string output)
        {
            Console.WriteLine(prefix + output);
            Logs.Add(prefix + output);

            if (!string.IsNullOrEmpty(file))
            {
                File.AppendAllText(file, prefix + output + Environment.NewLine);
            }
        }

        public void DebugLog(string output)
        {
            Log(output);
        }
    }
}
