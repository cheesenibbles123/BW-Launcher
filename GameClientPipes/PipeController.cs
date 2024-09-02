using System.Collections.Generic;
using System;
using System.IO;
using System.IO.Pipes;
using BWModLoader;
using UnityEngine;

namespace GameClientPipes
{

    enum MessageType
    {
        Achievement
    }

    [Mod]
    public class PipeController : MonoBehaviour
    {
        const string PipeName = "bw-modlauncher-named-pipe-main";

        public PipeController Instance;

        readonly Dictionary<MessageType, string> MessagePrefixLookup = new Dictionary<MessageType, string> {
            { MessageType.Achievement, "Achievement" },
        };

        private NamedPipeClientStream pipeStream;
        private StreamReader reader;
        private StreamWriter writer;

        void Awake()
        {
            if (!Instance)
            {
                Instance = this;
            }
            else
            {
                DestroyImmediate(this);
            }
        }

        void Start()
        {
            pipeStream = new NamedPipeClientStream(".", PipeName, PipeDirection.InOut);
            Debug.Log("Attempting to connect to pipe...");
            pipeStream.Connect();
            Debug.Log("Connected to pipe.");
            reader = new StreamReader(pipeStream);
            writer = new StreamWriter(pipeStream);
            writer.AutoFlush = true;
        }

        public string UpdateAchievement(string achievementID, int newScore, bool isNowComplete)
        {
            return DispatchMessage($"{MessagePrefixLookup[MessageType.Achievement]} {achievementID} {newScore} {isNowComplete}");
        }

        private string DispatchMessage(string message)
        {
            writer.WriteLine(message);
            return reader.ReadLine(); // Server response
        }
    }
}
