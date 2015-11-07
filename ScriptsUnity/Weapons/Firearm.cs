using UnityEngine;
using UnityEngine.UI;

public class Firearm : Weapon
{
    public int damagePerShot = 20;
    public float timeBetweenBullets = 0.15f;
    public float range = 100f;

    RaycastHit shootHit;
    int shootableMask;
    ParticleSystem gunParticles;
    LineRenderer gunLine;
    AudioSource gunAudio;
    Light gunLight;
    float effectsDisplayTime = 0.2f;

    Transform barrel;

    void Awake ()
    {
        player = transform.GetComponentInParent<PlayerController>();
        barrel = transform.FindChild("GunBarrelEnd");

        shootableMask = LayerMask.GetMask ("Shootable");
        
        gunLine = GetComponent <LineRenderer> ();
        gunAudio = GetComponent<AudioSource> ();

        gunLight = barrel.GetComponent<Light> ();
        gunParticles = barrel.GetComponent<ParticleSystem>();
    }


    void Update ()
    {
        timer += Time.deltaTime;
        
        if (timer >= timeBetweenBullets * effectsDisplayTime)
        {
            DisableEffects();
        }
    }
    

    public void DisableEffects ()
    {
        gunLine.enabled = false;
        gunLight.enabled = false;
    }


    public override void MainUse()
    {

        if (timer < timeBetweenBullets) return;

        timer = 0f;

        gunAudio.Play ();

        gunLight.enabled = true;

        gunParticles.Stop ();
        gunParticles.Play ();

        gunLine.enabled = true;
        gunLine.SetPosition(0, barrel.position);

        shootRay.origin = barrel.position;
        shootRay.direction = barrel.forward;

        if(Physics.Raycast (shootRay, out shootHit, range, shootableMask))
        {
            IDamageable targetHealth = (IDamageable)shootHit.collider.GetComponent(typeof(IDamageable));

            if (targetHealth != null && !targetHealth.IsDead() && player.GetTeam() != targetHealth.GetTeam())
            {
                targetHealth.ReceiveDamage(damagePerShot/*, shootHit.point*/);
            }
            gunLine.SetPosition (1, shootHit.point);
        }
        else
        {
            gunLine.SetPosition (1, shootRay.origin + shootRay.direction * range);
        }
    }

    
}
