using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    private bool isAttack = false;
    private float movedSpeed = 10.0f;
    private float jumpPower = 8.5f;

    public Transform groundChecker;
    public float groundRadius = 0.2f;
    public LayerMask groundLayer;

    private Rigidbody2D rigid;
    bool isGround = true;

    private Animator anim;

    // Start is called before the first frame update
    void Start()
    {
        rigid = GetComponent<Rigidbody2D>();
        anim = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.LeftControl))
        {
            isAttack = true;
            anim.SetBool("isAttack", true);
            SoundManager.instance.AttackSound();

            //단일 공격
            Collider2D col = Physics2D.OverlapCircle(transform.position,
                2, (1 << LayerMask.NameToLayer("Enemy")));
            //전체 공격
            //Collider2D[] cols = Physics2D.OverlapCircleAll();
            //for (int i = 0; i < cols.Length; i++)
            //{
            //
            //}
            if (col != null)
            {
                //col.GetComponent<Enemy>().Die();
            }
        }
        if (Input.GetKeyUp(KeyCode.LeftControl))
        {
            isAttack = false;
            anim.SetBool("isAttack", false);
        }

        isGround = Physics2D.OverlapCircle(groundChecker.position, groundRadius, groundLayer);
    }

    void FixedUpdate()
    {
        Move();
        Jump();
    }

    void Jump()
    {
        if (Input.GetKeyDown(KeyCode.W) && isGround)
        {
            rigid.AddForce(Vector2.up * jumpPower, ForceMode2D.Impulse);
            isGround = false;
        }
    }

    void Move()
    {
        float posX = Input.GetAxis("Horizontal");
        if (posX != 0)
        {
            if (posX >= 0)
            {
                transform.eulerAngles = new Vector3(0, 0, 0);
            }
            else
            {
                transform.eulerAngles = new Vector3(0, 180, 0);
            }
        }
        transform.Translate(Mathf.Abs(posX) * Vector3.right * movedSpeed * Time.deltaTime);
    }

    void OnTriggerStay2D(Collider2D coll)
    {
        if(isAttack && coll.gameObject.tag.Equals("Enemy")){
            coll.gameObject.GetComponent<Enemy>().Die();
        }
    }
}
