using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class rotation : MonoBehaviour
{
    float p;
    // Start is called before the first frame update
    void Start()
    {
        p = 1f;
    }

    // Update is called once per frame
    void Update()
    {
        transform.Rotate(0, 0, Random.Range(1f, 5f), Space.World);
        if(p > 2)
        {
            p = -1f;
        }
        else if (p < -2)
        {
            p = 1f;
        }
        transform.localScale += new Vector3(0, p * 0.01f , p * 0.01f);
        if(p < 0)
        {
            p-= 0.05f;
        }
        else if(p> 0)
        {
            p+= 0.05f;
        }
    }
}
