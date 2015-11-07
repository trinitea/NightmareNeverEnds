using UnityEngine;
using System.Collections;

public class SceneParameters {

    private static SceneParameters instance;

    private SceneParameters() { }

    int numberOfPlayer = 1;

    public static SceneParameters Instance
    {
        get
        {
            if (instance == null)
            {
                instance = new SceneParameters();
            }
            return instance;
        }
    }

    public int NumberOfPlayer { get; set; }


}
