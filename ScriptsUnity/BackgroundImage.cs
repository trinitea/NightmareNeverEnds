using UnityEngine;
using System.Collections;

public class BackgroundImage : MonoBehaviour {

    SceneController sc;

    void Start()
    {
        sc = GameObject.FindGameObjectWithTag("GameController").GetComponent<SceneController>();
    }

    public void CallModelChange()
    {
        sc.ShowModel();
    }
}
