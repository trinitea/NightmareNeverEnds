using UnityEngine;
using System.Collections;

public interface IHitManager{

    void ManageHit();
    //void ManageHit(Vector3 hitPoint);
    Team GetTeam();
}
