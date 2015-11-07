using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ScoreManager : MonoBehaviour
{
    public static int score;


    Text text;


    // test
    SceneParameters sp;


    void Awake ()
    {
        text = GetComponent <Text> ();
        score = 0;

        sp = SceneParameters.Instance;
    }


    void Update ()
    {
        text.text = "Score: " + sp.NumberOfPlayer;
    }
}
