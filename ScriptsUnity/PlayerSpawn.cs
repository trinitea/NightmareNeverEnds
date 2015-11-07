using UnityEngine;
using System.Collections;

public class PlayerSpawn : MonoBehaviour {

    float radius = 5f;
    float spawnProportion = 1f;

    int numberOfPlayer = 5;

    public Vector3 RetrieveSpawnPosition(int spawnNumber)
    {
        if(spawnNumber < 1 || spawnNumber > numberOfPlayer) return transform.position;

        return transform.position;
    }

    void OnDrawGizmosSelected (){
        Gizmos.color = new Color(0.05f,0.75f,0.05f,0.5f);
        Gizmos.DrawSphere(transform.position, radius);
    }

}
