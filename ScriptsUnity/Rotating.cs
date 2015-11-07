using UnityEngine;
using System.Collections;

public class Rotating : MonoBehaviour {

    public float speed = 1f;
    Vector3 basePosition;
    Vector3 baseRotation;

    public float timeBetweenReset = 5f;
    float resetTimer;

    void Start()
    {
        basePosition = transform.position;
        resetTimer = 0;

        
    }

	// Update is called once per frame
	void Update () {

        float x = -Mathf.Sin(Time.time * speed - 0.07f) * 5.5f;
        float z = -Mathf.Cos(Time.time * speed - 0.07f) * 5.5f;

        transform.position = new Vector3(basePosition.x + x, basePosition.y, basePosition.z + z);
        transform.LookAt(new Vector3(0, basePosition.y-2.7f, 0));
	}

    public void reset()
    {
        Debug.Log(basePosition);
        transform.position = basePosition;
        resetTimer = 0f;
    }
}
