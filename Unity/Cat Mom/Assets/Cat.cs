using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cat : MonoBehaviour
{
    float full = 5.0f;
    float energy = 0.0f;
    bool isFull = false;
    public int type = 0;
    // Start is called before the first frame update
    void Start()
    {
        float x = Random.Range(-7.0f, 7.0f);
        float y = 30.0f;
        transform.position = new Vector3(x, y, 0);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(energy < full)
        {
            if(type == 0)
            {
                transform.position += new Vector3(0.0f, -0.1f, 0.0f);
            }
            else if(type == 1)
            {
                transform.position += new Vector3(0.0f, -0.07f, 0.0f);
            }
            else
            {
                transform.position += new Vector3(0.0f, -0.15f, 0.0f);
            }
            if(transform.position.y < -16.0f)
            {
                GameManager.i.GameOver();
            }
        }
        else
        {
            if(!isFull)
                GameManager.i.addCat();
            isFull = true;
            if(transform.position.x > 0)
                transform.position += new Vector3(0.05f, 0, 0);
            else
                transform.position += new Vector3(-0.05f, 0, 0);
            Destroy(gameObject, 3.0f);
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.tag == "food")
        {
            if(energy < full)
            {
                energy += 1.0f;
                Destroy(collision.gameObject);
                gameObject.transform.Find("Hungry/Canvas/Front").transform.localScale = new Vector3(energy / full, 1.0f, 1.0f);
                if(energy >= full)
                {
                    gameObject.transform.Find("Hungry").gameObject.SetActive(false);
                    gameObject.transform.Find("Full").gameObject.SetActive(true);
                }
            }
        }
    }
}
