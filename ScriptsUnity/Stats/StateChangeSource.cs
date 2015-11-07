using UnityEngine;
using System.Collections;

public class StateChangeSource{

    GameObject source;

    float amount;
    
    public StateChangeSource(float amount) : this(amount,null) { }

    public StateChangeSource(float amount, GameObject source){
        Source = source;
        Amount = amount;
    }

    public GameObject Source
    {
        get { return source; }
        set { source = value; }
    }

    public float Amount
    {
        get { return amount; }
        set { amount = value; }
    }
}
