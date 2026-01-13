using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ModLoader
{
    internal class CommunicationPipes
    {
        private static TcpClient clientSocket;

        public static void init()
        {
            clientSocket = new TcpClient();
            clientSocket.Connect("127.0.0.1", 54000);

            sendMessage("Hello from C# client!");
        }

        public static void sendMessage(string message)
        {
            UnityEngine.Debug.Log("Sending message: " + message);
            NetworkStream stream = clientSocket.GetStream();
            byte[] data = Encoding.UTF8.GetBytes(message);
            stream.Write(data, 0, data.Length);
        }

        public void terminate()
        {
            clientSocket.Close();
        }
    }
}
