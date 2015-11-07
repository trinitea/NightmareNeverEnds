using UnityEngine;
using System.Collections;

public class Stat {

    int maxValue;
    float currentValue;

    public Stat(int maxValue) : this(maxValue, maxValue) { }

    public Stat(int maxValue, int startingValue)
    {
        MaxValue = maxValue;
        CurrentValue = startingValue;
    }

    public int MaxValue
    {
        get{ return maxValue; }
        set
        { 
            if(value < 0) value = 0;
            maxValue = value;
        }
    }

    public float CurrentValue
    {
        internal get { return currentValue; }
        set
        {
            if (value > maxValue) value = maxValue;
            currentValue = value;
        }
    }

    public float ChangeAmount(float amount)
    {
        return CurrentValue = amount + currentValue;
    }

    public bool IsFull()
    {
        return currentValue >= maxValue;
    }

    public bool IsEmpty()
    {
        return currentValue <= 0;
    }
}
