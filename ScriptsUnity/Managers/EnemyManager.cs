using UnityEngine;

public class EnemyManager : MonoBehaviour
{

    bool spawnMobs;

    public GameObject enemy;
    public float spawnTime = 3f;
    public Transform[] spawnPoints;


    void Start ()
    {
        spawnMobs = true;
        InvokeRepeating ("Spawn", spawnTime, spawnTime);
    }


    void Spawn ()
    {

        if (spawnMobs)
        {
            int spawnPointIndex = Random.Range (0, spawnPoints.Length);

            Instantiate (enemy, spawnPoints[spawnPointIndex].position, spawnPoints[spawnPointIndex].rotation);
        }
    }
}
