using System.Collections.Generic;
using System;
using BWModLoader;
using UnityEngine;
using System.Net.Sockets;
using System.Text;
using System.Diagnostics;
using System.Threading;

namespace GameClientPipes
{

    enum MessageType
    {
        Achievement
    }

    [Mod]
    public class ConnectionClient : MonoBehaviour
    {
        int port = 28015;
        string ip = "127.0.0.1";
        NetworkStream stream;
        TcpClient client;

        public ConnectionClient Instance;

        readonly Dictionary<MessageType, string> MessagePrefixLookup = new Dictionary<MessageType, string> {
            { MessageType.Achievement, "Achievement" },
        };

        void Awake()
        {}

        void Start()
        {
            try
            {
                client = new TcpClient(ip, port);
                UnityEngine.Debug.Log("Setup TCP client");

            }
            catch (System.Net.Sockets.SocketException e)
            {
                UnityEngine.Debug.Log("Connection Failed.. " + e.Message);
            }
            Thread thread = new Thread(sendHeartbeat);
            thread.IsBackground = true;
            thread.Start();
        }

        void sendHeartbeat()
        {
            while (true) { 
                DispatchMessage("heartbeat");
                Thread.Sleep(500);
            }
        }

        public void UpdateAchievement(string achievementID, int newScore, bool isNowComplete)
        {
            DispatchMessage($"{MessagePrefixLookup[MessageType.Achievement]} {achievementID} {newScore} {isNowComplete}");
        }

        private void DispatchMessage(string message)
        {
            try
            {
                int byteCount = Encoding.ASCII.GetByteCount(message);   
                byte[] data = new byte[byteCount];  
                data = Encoding.ASCII.GetBytes(message);    
                stream = client.GetStream();    
                stream.Write(data, 0, data.Length); 
            }catch (NullReferenceException e)
            {
                UnityEngine.Debug.Log("Error dispatching message: " + e.Message);
            }
        }

        void destroy()
        {
            stream.Close();
            client.Close();
        }
    }
}
