using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class dog : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        Vector3 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        float x = mousePos.x;
        if(x > 7f)
        {
            x = 7f;
        }
        if(x < -7f)
        {
            x = -7f;
        }
        transform.position = new Vector3(x, transform.position.y, 0);
    }
}
