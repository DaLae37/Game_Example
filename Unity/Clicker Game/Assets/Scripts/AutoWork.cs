using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AutoWork : MonoBehaviour
{
    public static long AutoMOneyIncreaseAmount = 0;
    public static long AutoMOneyIncreasePrice = 1000;
    static GameManager GM;
    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(Work());
    }

    IEnumerator Work()
    {
        while (true)
        {
            if (!GM)
            {
                GM = GameObject.Find("GameManager").GetComponent<GameManager>();
            }
            GM.Money += AutoMOneyIncreaseAmount;

            yield return new WaitForSeconds(1);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
