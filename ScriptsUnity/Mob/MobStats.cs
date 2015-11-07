using UnityEngine;

public class MobStats : MonoBehaviour, IDamageable
{
    public int mobHealth = 100;
    
    IHitManager controller;

    private Stat health;

    void Awake ()
    {
        controller = (IHitManager)GetComponent(typeof(IHitManager));

        health = new Stat(mobHealth);

    }

    public void ReceiveDamage(float amount/*, Vector3 hitPoint*/) // hitPoint will be available later
    {
        if (amount > 0)
        {
            health.ChangeAmount(-amount);
            //healthSlider.value = health.CurrentValue; //or anyother life bar
        }
        controller.ManageHit();
    }

    public bool IsDead()
    {
        return health.IsEmpty();
    }

    public Team GetTeam()
    {
        return controller.GetTeam();
    }
}
