using System;
using System.Collections.Generic;
using UnityEngine;

namespace Tools
{
public class MainThreadDispatcher : MonoBehaviour {
    private static readonly Queue<Action> queue = new Queue<Action>();
    private static MainThreadDispatcher instance;

    public static void Initialize() {
        if (instance != null) return;
        var obj = new GameObject("MainThreadDispatcher");
        DontDestroyOnLoad(obj); instance = obj.AddComponent<MainThreadDispatcher>();
    }

    public static void RunOnMainThread(Action action) {
        if (action == null) return;
        lock (queue) queue.Enqueue(action);
    }

    private void Update() {
        lock (queue) {
            while (queue.Count > 0) {
                var action = queue.Dequeue();
                action();
            }
        }
    }
}
}