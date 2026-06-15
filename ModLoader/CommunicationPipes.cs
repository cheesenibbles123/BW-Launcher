using System;
using System.Net.Sockets;
using System.Text;

namespace Tools
{
    public class CommunicationPipes
    {
        private static TcpClient clientSocket = null;

        public static void init()
        {
            if (clientSocket == null || !clientSocket.Connected)
            {
                if (clientSocket == null)
                {
                    clientSocket = new TcpClient();
                }

                try
                {
                clientSocket.Connect("127.0.0.1", 54000);

                AppDomain.CurrentDomain.ProcessExit += new EventHandler(OnProcessExit);

                sendMessage("Hello from game client!");

                } catch(Exception e)
                {
                    UnityEngine.Debug.Log("Got error setting up socket connection " + e.Message);
                }
            } else
            {
                UnityEngine.Debug.Log("Called CommunicationPipes.init() with a socket already present");
            }
        }

        /// <returns> Whether the message was successfully sent or not </returns>
        public static bool sendMessage(string message)
        {
            if (clientSocket != null && clientSocket.Connected)
            {
                UnityEngine.Debug.Log("Sending message to launcher: " + message);
                NetworkStream stream = clientSocket.GetStream();
                byte[] data = Encoding.UTF8.GetBytes(message);
                stream.Write(data, 0, data.Length);
                return true;
            }

            return false;
        }

        /**
         * <summary>
         * Currently "stats" require an achievement hooked up to them to function
         * </summary>
         * <returns> Whether the message was successfully sent or not </returns>
         */
        public static bool incrementAchievement(string modId, string achievementId, int progressValue)
        {
            if (clientSocket != null && clientSocket.Connected)
            {
                UnityEngine.Debug.Log("Sending achievement update to launcher: " + achievementId + " " + progressValue);
                NetworkStream stream = clientSocket.GetStream();
                byte[] data = Encoding.UTF8.GetBytes(string.Format("[ACHIEVEMENT][{0}][{1}][{2}]", modId, achievementId, progressValue));
                stream.Write(data, 0, data.Length);
                return true;
            }

            return false;
        }

        public static void CloseConnection()
        {
            if (clientSocket != null && clientSocket.Connected)
            {
                clientSocket.Close();
            }
        }

        private static void OnProcessExit(object sender, EventArgs e)
        {
            CloseConnection();
            clientSocket = null;
        }
    }
}
