using UnityEngine;
using System.Collections;

public class MobController : MonoBehaviour, IHitManager
{
    // Attack
    public float timeBetweenAttacks = 0.5f;
    public float timeBetweenTargetRefresh = 3.5f;
    public int attackDamage = 10;

    public float sinkSpeed = 2.5f;
    public int scoreValue = 10;
    public AudioClip damageClip;
    public AudioClip deathClip;

    //EnemyHealth enemyHealth;
    MobStats mobStats;
    NavMeshAgent nav;
    ParticleSystem hitParticles;
    CapsuleCollider capsuleCollider;
    Animator anim;
    AudioSource mobAudio;

    Team team;

    Transform aggroTarget;
    IDamageable damageableTarget;

    float attackTimer, TargetRefreshTimer;

    bool isDead; // Automata
    bool isSinking; // Automata

    void Awake ()
    {
        nav = GetComponent<NavMeshAgent>();
        mobStats = GetComponent<MobStats>();
        anim = GetComponent<Animator>();
        mobAudio = GetComponent<AudioSource>();
        hitParticles = GetComponentInChildren<ParticleSystem>();
        capsuleCollider = GetComponent<CapsuleCollider>();

        aggroTarget = GetNerestTarget();

        attackTimer = TargetRefreshTimer = 0f;
        team = Team.Mobs;
    }

    void Update()
    {
        // replace with state automata
        if (!mobStats.IsDead())
        {
            attackTimer += Time.deltaTime;
            TargetRefreshTimer += Time.deltaTime;

            if(TargetRefreshTimer >= timeBetweenTargetRefresh)
            {
                aggroTarget = GetNerestTarget();
                TargetRefreshTimer = 0f;
            }

            if (aggroTarget != null)
            {
                anim.SetBool("HasTarget",true);
                nav.SetDestination(aggroTarget.position);
            }
            else
            {
                anim.SetBool("HasTarget", false);
            }

            if (attackTimer >= timeBetweenAttacks && damageableTarget != null && !damageableTarget.IsDead())
            {
                Attack();
            }
        }

        // replace with state automata
        if(isSinking)
        {
            transform.position += Vector3.down * sinkSpeed * Time.deltaTime;
        }
    }

    void OnTriggerEnter (Collider other)
    {
        IDamageable damageable = (IDamageable)other.gameObject.GetComponent(typeof(IDamageable));

        if (damageable != null && damageable.GetTeam() != team && damageable.GetTeam() != Team.Neutral)
        {
            if (damageableTarget != null)
            {
                // change aggro ??
            }
            damageableTarget = damageable;
        }
    }

    void OnTriggerExit (Collider other)
    {

        if(damageableTarget != null)
        {
            IDamageable damageable = (IDamageable)other.gameObject.GetComponent(typeof(IDamageable));
            if (damageableTarget == damageable)
            {
                damageableTarget = null;
            }
        }
    }

    void Attack()
    {
        if(!mobStats.IsDead()){
            attackTimer = 0f;

            damageableTarget.ReceiveDamage(attackDamage);
            if (damageableTarget.IsDead())
            {
                aggroTarget = null;
                damageableTarget = null;
            }
        }
    }

    public Team GetTeam()
    {
        return team;
    }

    #region health
    public void ManageHit()
    {

        if (mobStats.IsDead())
        {
            hitParticles.Play();
            Die();

        }
        else
        {
            //hitParticles.transform.position = hitPoint;
            hitParticles.Play();
            mobAudio.Play();
        }
    }

    void Die()
    {

        GetComponent<NavMeshAgent>().enabled = false;

        capsuleCollider.isTrigger = true;

        anim.SetTrigger("Dead");

        mobAudio.clip = deathClip;
        mobAudio.Play();
    }

    public void StartSinking()
    {
        GetComponent<Rigidbody>().isKinematic = true;
        isSinking = true;
        //ScoreManager.score += scoreValue;
        Destroy(gameObject, 2f);
    }
    #endregion

    Transform GetNerestTarget() // possible strategy
    {
        GameObject[] possibleTargets = GameObject.FindGameObjectsWithTag(Tags.Player);
        ArrayList possiblePlayers = new ArrayList();

        foreach (GameObject go in possibleTargets)
        {
            if(go.GetComponent<PlayerController>() != null)
            {
                possiblePlayers.Add(go.GetComponent<PlayerController>());
            }
        }

        if(possiblePlayers.Count > 0)
        {
            Transform current = null;

            foreach (PlayerController pc in possiblePlayers)
            {
                if (
                        !pc.IsDead() &&
                        (current == null|| 
                        Vector3.Distance(transform.position, pc.transform.position) <
                        Vector3.Distance(transform.position, transform.position)
                    )
                )
                {
                    current = pc.transform;
                }
            }

            return current;
        }
        else
        {
            return null;
        }
    }
    

    
}
