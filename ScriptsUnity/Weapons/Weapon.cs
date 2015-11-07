using UnityEngine;
using System.Collections;

public abstract class Weapon : MonoBehaviour, IWeapon {

    protected PlayerController player;

    protected float timer;
    protected Ray shootRay;

    public GameObject UIElement; 

    public virtual void MainUse()
    {
        Debug.Log("Weapon main usage not implemented");
    }

    public virtual void SecondaryUse()
    {
        Debug.Log("Weapon secondary usage not implemented");
    }
    
    bool CheckTeam()
    {
        return true;
    }

}
