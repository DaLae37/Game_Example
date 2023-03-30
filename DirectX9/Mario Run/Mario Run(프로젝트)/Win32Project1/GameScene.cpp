#include "GameScene.h"
#include"Global.h"
#include "MainScene.h"
#include <time.h>
#include <stdlib.h>
GameScene::GameScene(void)
{
	player = new Player();
	AddObject(player);
	player->pos = Vec2(100, 430);
	backDrow = false;
	dropDrow = true;
	Background* back = new Background();
	AddObject(back);
	back->pos = Vec2(0, 0);
	backList.push_back(back);
	Map *map = new Map();
	AddObject(map);
	map->pos = Vec2(0, 500);
	mapList.push_back(map);
	DropDelay = 0.5f;
	DropDelayTimer = 0;
	CoinDelay = 0.1f;
	CoinDelayTimer = 0;
	number1 = new Number();
	number2 = new Number();
	number3 = new Number();
	number4 = new Number();
	Score = 0;
	AddObject(number1);
	AddObject(number2);
	AddObject(number3);
	AddObject(number4);
	number1->pos = Vec2(160, 10);
	number2->pos = Vec2(110, 10);
	number3->pos = Vec2(60, 10);
	number4->pos = Vec2(10, 10);
	isGameOver = false;
	upCoin = 0;
	up = false;
	InstDelayTime = 0.2f;
	InstDelayTimer = 0;
}


GameScene::~GameScene(void)
{
}

void GameScene::Render()
{

	//sprintf(str, "%d", Score);
	for (auto& background : backList)
	{
		background->Render();
	}
	for (auto& building : buildingList)
	{
		building->Render();
	}
	for (auto& map : mapList)
	{
		map->Render();
	}
	for (auto& coin : coinList)
	{
		coin->Render();
	}
	for (auto& drop : dropList)
	{
		drop->Render();
	}
	player->Render();
	number1->Render();
	number2->Render();
	number3->Render();
	number4->Render();

	number1->num[Score%10]->Render();
	number2->num[Score/10%10]->Render();
	number3->num[Score/100%10]->Render();
	number4->num[Score/1000]->Render();
}

void GameScene::Update(float dTime) {
	InstDelayTimer += dTime;
	DropDelayTimer += dTime;
	CoinDelayTimer += dTime;
	Scene::Update(dTime);
	if (CoinDelayTimer > CoinDelay)
	{
		CoinDelayTimer = 0;
		InstCoin();
	}
	if (dropDrow == false)
	{
		int i;
		srand((unsigned int)time(NULL));
		i = rand() % 10 + 1;
		if (i <= 7 && DropDelayTimer>DropDelay)
		{
			up = true;
			InstDrop();
		}
		else if (DropDelayTimer > DropDelay)
		{
			up = true;
			InstBuilding();
		}
		dropDrow = true;
		DropDelayTimer = 0;
	}
	for (auto& drop : dropList)
	{
		if (IsCollisionRect(drop->pos, player->pos, drop->rect, player->rect))
		{
			isGameOver = true;
		}
	}
	for (auto& building : buildingList)
	{
		if (IsCollisionRect(building->pos, player->pos, building->rect, player->rect))
		{
			isGameOver = true;
		}
	}
	for (auto iter = coinList.begin(); iter != coinList.end(); iter++)
	{
		if (IsCollisionRect((*iter)->pos, player->pos, (*iter)->rect, player->rect))
		{
			Score += 1;
			RemoveObject(*iter);
			delete(*iter);
			cout << "코인 삭제" << endl;
			iter = coinList.erase(iter);
			if (iter == coinList.end())
			{
				break;
			}
		}
	}
	if (backDrow == false)
	{
		InstBack();
	}
	for (auto iter = backList.begin(); iter != backList.end(); iter++)
	{
		if ((*iter)->pos.x < -SCREEN_WIDTH+100)
		{
			RemoveObject(*iter);
			delete(*iter);
			backDrow = false;
			dropDrow = false;
			cout << "백그라운드 삭제" << endl;
			iter = backList.erase(iter);
			if (iter == backList.end())
			{
				break;
			}
		}
	}
	for (auto iter = buildingList.begin(); iter != buildingList.end(); iter++)
	{
		if ((*iter)->pos.x < -SCREEN_WIDTH + 100)
		{
			RemoveObject(*iter);
			delete(*iter);
			cout << "건물 삭제" << endl;
			iter = buildingList.erase(iter);
			if (iter == buildingList.end())
			{
				break;
			}
		}
	}
	for (auto iter = mapList.begin(); iter != mapList.end(); iter++)
	{
		if ((*iter)->pos.x < -SCREEN_WIDTH + 100)
		{
			RemoveObject(*iter);
			delete(*iter);
			cout << "다리 삭제" << endl;
			iter = mapList.erase(iter);
			if (iter == mapList.end())
			{
				break;
			}
		}
	}
	for (auto iter = dropList.begin(); iter != dropList.end(); iter++)
	{
		if ((*iter)->pos.x < -SCREEN_WIDTH + 100)
		{
			RemoveObject(*iter);
			delete(*iter);
			cout << "히오스 삭제" << endl;
			iter = dropList.erase(iter);
			if (iter == dropList.end())
			{
				break;
			}
		}
	}
	for (auto iter = coinList.begin(); iter != coinList.end(); iter++)
	{
		if ((*iter)->pos.x < -SCREEN_WIDTH + 100)
		{
			RemoveObject(*iter);
			delete(*iter);
			cout << "코인 삭제" << endl;
			iter = coinList.erase(iter);
			if (iter == coinList.end())
			{
				break;
			}
		}
	}
	if (IsPointInRect(player->pos, player->rect, GetMousePos()) && GetMyKeyState(VK_LBUTTON)==1)
	{
		isGameOver = true;
	}
	if (isGameOver == true)
	{
		sceneManager->ChangeScene(new MainScene);
	}
}

void GameScene::InstBack()
{
	Background* back = new Background();
	AddObject(back);
	Map *map = new Map();
	AddObject(map);
	back->pos = Vec2(SCREEN_WIDTH - 100, 0);
	map->pos = Vec2(SCREEN_WIDTH, 500);
	backDrow = true;
	backList.push_back(back);
	mapList.push_back(map);
}

void GameScene::InstDrop()
{
	Drop *drop = new Drop();
	AddObject(drop);
	drop->pos = Vec2(SCREEN_WIDTH+170, 500);
	dropList.push_back(drop);
}

void GameScene::InstCoin()
{
	Coin *coin = new Coin();
	AddObject(coin);
	if (up == true && upCoin <= 4)
	{
		coin->pos = Vec2(SCREEN_WIDTH, 300);
		upCoin++;
		if (upCoin == 4)
		{
			upCoin = 0;
			up = false;
		}
	}
	else
	{
		coin->pos = Vec2(SCREEN_WIDTH, 400);
	}
	coinList.push_back(coin);
}

void GameScene::InstBuilding()
{
	Building *building = new Building();
	AddObject(building);
	building->pos = Vec2(SCREEN_WIDTH+170, 450);
	buildingList.push_back(building);
}