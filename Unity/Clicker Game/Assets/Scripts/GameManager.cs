using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class GameManager : MonoBehaviour
{
    public long Money;
    public long MoneyincreaseAmount;
    public GameObject prefabMoney;
    public Text textMoney;
    public Text textPrice;
    public long moneyIncreaseLevel;
    public long moneyIncreasePrice;
    public Button buttonRecurit;
    public Text textRecruit;
    public Text textPerson;
    public int employeeCount;
    public int width;
    public float space;

    public GameObject prefabEmployee;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        ShowInfo();
        MoneyIncrease();
        ButtonRecuritActiveCheck();
        UpdatePanelText();
        UpdateRecruitPannelText();
    }

    void ShowInfo()
    {
        if(Money == 0)
        {
            textMoney.text = "0 ��";
        }
        else
        {
            textMoney.text = Money.ToString("###.###") + " ��";
        }

        if(employeeCount == 0)
        {
            textPerson.text = "0 ��";
        }
        else
        {
            textPerson.text = employeeCount + " ��";
        }
    }

    void MoneyIncrease()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if(EventSystem.current.IsPointerOverGameObject() == false)
            {
                Money += MoneyincreaseAmount;
                Vector2 mousePosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
                Instantiate(prefabMoney, mousePosition, Quaternion.identity);
            }
        }
    }

    public void UpgradePrice()
    {
        if (Money >= moneyIncreasePrice)
        {
            Money -= moneyIncreasePrice;
            moneyIncreaseLevel += 1;
            MoneyincreaseAmount += moneyIncreaseLevel * 100;
            moneyIncreasePrice = moneyIncreaseLevel * 500;
        }
    }

    void UpdatePanelText()
    {
        textPrice.text = "Lv." + moneyIncreaseLevel + "�ܰ���� \n\n";
        textPrice.text += "���� �� �ܰ� >\n";
        textPrice.text += "Lv." + MoneyincreaseAmount.ToString("###.###") + "��\n";
        textPrice.text += "���׷��̵� ���� >\n";
        textPrice.text += "Lv." + moneyIncreasePrice.ToString("###.###") + "��\n";
    }

    void UpdateRecruitPannelText()
    {
        textRecruit.text = "Lv." + employeeCount + "������� \n\n";
        textRecruit.text += "���� 1�� �� �ܰ� >\n";
        textRecruit.text += "Lv." + AutoWork.AutoMOneyIncreaseAmount.ToString("###.###") + "��\n";
        textRecruit.text += "���׷��̵� ���� >\n";
        textRecruit.text += "Lv." + AutoWork.AutoMOneyIncreasePrice.ToString("###.###") + "��\n";
    }

    void ButtonRecuritActiveCheck()
    {
        if(Money >= AutoWork.AutoMOneyIncreasePrice)
        {
            buttonRecurit.interactable = true;
        }
        else
        {
            buttonRecurit.interactable = false;
        }
    }

    public void UpgradeRecruit()
    {
        if(Money >= AutoWork.AutoMOneyIncreasePrice)
        {
            Money -= AutoWork.AutoMOneyIncreasePrice;
            employeeCount += 1;
            AutoWork.AutoMOneyIncreaseAmount += moneyIncreaseLevel * 5;
            AutoWork.AutoMOneyIncreasePrice += employeeCount * 500;
            CreateEmployee();
        }
    }

    void CreateEmployee()
    {
        Vector2 bossSpot = GameObject.Find("Employee").transform.position;
        float spotX = bossSpot.x + (employeeCount % width) * space;
        float spotY = bossSpot.y - (employeeCount / width) * space;

        Instantiate(prefabEmployee, new Vector3(spotX, spotY, -1), Quaternion.identity);
        //GameObject obj = Instantiate(prefabDesk, new Vector2(spotX, spotY));
    }
}
