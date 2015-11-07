using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class Nuke : Weapon 
{

    public int nukeDamage = 100;
    public float nukeRadius = 10f;

    public float nukeTreshold = 100f;
    public float nukeFillRate = 3f;

    public GameObject explosion;
    public Slider nukeSlider;

    int shootableMask;
    float nukeCharge;

    void Awake()
    {
        player = transform.GetComponentInParent<PlayerController>();

        shootableMask = LayerMask.GetMask("Shootable");

        nukeSlider.maxValue = nukeTreshold;
        nukeSlider.value = nukeCharge = 0;
    }

	void Update ()
    {

        FillNuke(Time.deltaTime * nukeFillRate);
	}

    /*
     * Weapon Main Action
     */
    public override void MainUse()
    {
        if (nukeCharge < nukeTreshold) return;

        nukeSlider.value = nukeCharge = 0;

        Instantiate(explosion, transform.position, transform.rotation);

        shootRay.origin = transform.position;
        shootRay.direction = transform.forward;

        foreach (Collider hit in Physics.OverlapSphere(transform.position, nukeRadius, shootableMask))
        {
            IDamageable targetHealth = (IDamageable)hit.collider.GetComponent(typeof(IDamageable));
            if (targetHealth != null && !targetHealth.IsDead() && targetHealth.GetTeam() != player.GetTeam())
            {
                targetHealth.ReceiveDamage(nukeDamage/*, hit.transform.position*/);
            }
        }
    }

    void FillNuke(float fill)
    {
        nukeCharge += fill;

        if (nukeCharge > nukeTreshold)
        {
            nukeCharge = nukeTreshold;
        }

        nukeSlider.value = nukeCharge;
    }
}
