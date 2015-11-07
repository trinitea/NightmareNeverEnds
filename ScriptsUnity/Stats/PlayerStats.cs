using UnityEngine;
using UnityEngine.UI;

public class PlayerStats : MonoBehaviour, IDamageable/*, IHealeable*/
{

    public int maxHealth = 100;
    public int startingHealth = 100;
    /*
    public bool hasStamina;
    public int maxStamina = 100;
    public int startingStamina = 100;
    */
    Stat health;
    //Stat stamina;

    Slider healthSlider;
    IHitManager player;

    void Awake()
    {
        player = (IHitManager)GetComponent(typeof(IHitManager));

        health = new Stat(maxHealth, startingHealth);
        //stamina = new Stat(maxStamina, startingStamina);
    }

    public Slider Slider
    {
        set
        {
            healthSlider = value;

            healthSlider.maxValue = startingHealth;
            healthSlider.value = health.CurrentValue;
        }
    }

    public void ReceiveDamage(float amount) // DamageCounter
    {
        if (amount > 0)
        {
            health.ChangeAmount(-amount);
            healthSlider.value = health.CurrentValue;
        }
        player.ManageHit();
    }

    public bool IsDead()
    {
        return health.IsEmpty();
    }

    public Team GetTeam()
    {
        return player.GetTeam();
    }

    public void Heal(float healthAmount)
    {
        if (healthAmount > 0)
        {
            health.ChangeAmount(healthAmount);
            healthSlider.value = health.CurrentValue;
        }
    }

}
