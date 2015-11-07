using UnityEngine;
using System.Collections;
public interface IDamageable {

    void ReceiveDamage(float amount);
    //void Heal(float amount);
    Team GetTeam();
    bool IsDead();
}
