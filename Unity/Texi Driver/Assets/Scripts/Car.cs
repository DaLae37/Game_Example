using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Car : MonoBehaviour
{
    float speed = 0;
    Vector2 startPos;
    void Start()
    {
    }
    void Update()
    {
        // �������� ���̸� ���Ѵ�
        if (Input.GetMouseButtonDown(0))
        {
            // ���콺�� Ŭ���� ��ǥ
            this.startPos = Input.mousePosition;
        }
        else if (Input.GetMouseButtonUp(0))
        {
            // ���콺 ��ư���� �հ����� ������ �� ��ǥ
            Vector2 endPos = Input.mousePosition;
            float swipeLength = endPos.x - this.startPos.x;
            // �������� ���̸� ó�� �ӵ��� ��ȯ�Ѵ�
            this.speed = swipeLength / 5000.0f;
            // ȿ������ ���
            GetComponent<AudioSource>().Play();
        }
        transform.Translate(this.speed, 0, 0); // �̵�
        this.speed *= 0.98f;
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if(collision.gameObject.tag == "Finish")
        {
            SceneManager.LoadScene("SampleScene");
        }
    }
}