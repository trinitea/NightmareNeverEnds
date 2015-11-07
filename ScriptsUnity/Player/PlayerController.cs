using UnityEngine;
using UnityEngine.UI;

public class PlayerController : MonoBehaviour, IHitManager
{
    public float speed = 6.0f;

    Vector3 movement;
    Animator anim;

    Rigidbody playerRigidBody;

    // vision related
    int floorMask;
    float camRayLength = 100f;
    PlayerCamera playerCamera;

    // action related
    public Weapon mainWeapon;
    public Weapon secondaryWeapon;

    // stats
    PlayerStats playerStats;
    Team team = Team.Allies;
    bool damaged;

    public Image damageImage;
    public AudioClip damageClip;
    public AudioClip deathClip;
    public float flashSpeed = 5f;
    public Color flashColour = new Color(1f, 0f, 0f, 0.1f);

    AudioSource playerAudio;

    void Awake()
    {
        playerRigidBody = GetComponent<Rigidbody>();
        playerStats = GetComponent<PlayerStats>();
        anim = GetComponent<Animator>();
        playerAudio = GetComponent<AudioSource>();

        floorMask = LayerMask.GetMask("Floor");

        damaged = false;

        InitCamera();
    }

    void Update()
    {

        if (!playerStats.IsDead())
        {
            if (Input.GetButton("Fire1") && mainWeapon != null)
            {
                mainWeapon.MainUse();
            }

            if (Input.GetButton("Fire2"))
            {
                if (secondaryWeapon != null)
                {
                    secondaryWeapon.MainUse();
                }
                else if (mainWeapon != null)
                {
                    mainWeapon.SecondaryUse();
                }

            }

            DamageCheck();
        }
    }

    void FixedUpdate()
    {
        if (!playerStats.IsDead())
        {

            float h = Input.GetAxisRaw("Horizontal");
            float v = Input.GetAxisRaw("Vertical");

            Move(h, v);
            Turning();
            Animating(h, v);
        }
    }

    public Team GetTeam()
    {
        return team;
    }

    public bool IsDead() // very bad
    {
        return playerStats.IsDead();
    }

    #region vision
    public PlayerCamera PlayerCamera { get; set; }

    void InitCamera()
    {
        if (playerCamera != null)
        {
            GameObject healthBar = Instantiate(Resources.Load("HealthBar", typeof(GameObject))) as GameObject;
            playerCamera.AddUIElement(healthBar);

            playerStats.healthSlider = healthBar.GetComponentInChildren<Slider>();
        }
    }

    #endregion

    #region movement
    void Move(float h, float v)
    {
        movement.Set(h, 0f, v);

        movement = movement.normalized * speed * Time.deltaTime;
        playerRigidBody.MovePosition(transform.position + movement);
    }

    void Turning()
    {
        Ray camRay = Camera.main.ScreenPointToRay(Input.mousePosition);

        RaycastHit floorHit;

        if (Physics.Raycast(camRay, out floorHit,camRayLength,floorMask))
        {
            Vector3 playerToMouse = floorHit.point - transform.position;
            playerToMouse.y = 0;

            Quaternion newRotation = Quaternion.LookRotation(playerToMouse);
            playerRigidBody.MoveRotation(newRotation);
        }
    }

    void Animating(float h, float v)
    {
        bool walking = h != 0f || v != 0f;
        anim.SetBool("IsWalking",walking);
    }
    #endregion

    #region Health
    public void ManageHit() // TODO: Delegate ? to send to player stat whata to do when hit
    {
        if(playerStats.IsDead())
        {
            anim.SetTrigger("Die");
            playerAudio.clip = deathClip;
            
        }
        else
        {
            damaged = true;
            playerAudio.clip = damageClip;
        }
        playerAudio.Play();
    }

    void DamageCheck()
    {
        if (damaged)
        {
            damageImage.color = flashColour;
        }
        else
        {
            damageImage.color = Color.Lerp(damageImage.color, Color.clear, flashSpeed * Time.deltaTime);
        }
        damaged = false;
    }

    #endregion
}
