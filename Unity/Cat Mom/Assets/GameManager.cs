using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class GameManager : MonoBehaviour
{
    public GameObject food;
    public GameObject dog;
    public GameObject cat;
    public GameObject fatCat;
    public static GameManager i;
    public GameObject reBUtton;
    public Text text;
    public Image image;
    int level = 0;
    int catNum = 0;

    private void Awake()
    {
        i = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("makeFood", 0.0f, 0.2f);
        InvokeRepeating("makeCat", 0.0f, 1f);
    }

    void makeFood()
    {
        float x = dog.transform.position.x;
        float y = dog.transform.position.y + 2.0f;
        Instantiate(food, new Vector3(x, y, 0), Quaternion.identity);
    }

    void makeCat()
    {
        if(level < 2)
        {
            float p = Random.Range(0,10);
            if(p < 8)
            {
                Instantiate(cat);
            }
        }
        else if (level >= 2)
        {
            float p = Random.Range(0, 10);
            if (p < 2)
            {
                Instantiate(cat);
            }
            else
            {
                Instantiate(fatCat);
            }
        }
    }

    public void GameOver()
    {
        reBUtton.SetActive(true);
        Time.timeScale = 0f;
    }

    public void addCat()
    {
        catNum += 1;
        level = catNum / 5;
        
        image.transform.localScale = new Vector3((catNum % 5) / 5f, 1, 1);
        text.text = level.ToString();
    }
}
