using System.Net.Sockets;
using System.Text;

namespace Tools
{
    public class CommunicationPipes
    {
        private static TcpClient clientSocket;

        public static void init()
        {
            clientSocket = new TcpClient();
            clientSocket.Connect("127.0.0.1", 54000);

            sendMessage("Hello from game client!");
        }

        public static void sendMessage(string message)
        {
            UnityEngine.Debug.Log("Sending message to launcher: " + message);
            NetworkStream stream = clientSocket.GetStream();
            byte[] data = Encoding.UTF8.GetBytes(message);
            stream.Write(data, 0, data.Length);
        }

        public static void incrementAchievement(string modId, string achievementId, int progressValue)
        {
            UnityEngine.Debug.Log("Sending achievement update to launcher: " + achievementId + " " + progressValue);
            NetworkStream stream = clientSocket.GetStream();
            byte[] data = Encoding.UTF8.GetBytes(string.Format("[ACHIEVEMENT][{0}][{1}][{2}]", modId, achievementId, progressValue));
            stream.Write(data, 0, data.Length);
        }

        public void terminate()
        {
            clientSocket.Close();
        }
    }
}
