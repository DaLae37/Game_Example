#pragma once
#include "scene.h"
#include "Player.h"
#include "Map.h"
#include "Drop.h"
#include "Background.h"
#include "building.h"
#include "Coin.h"
#include "Number.h"

class GameScene :
	public Scene
{
public:
	GameScene(void);
	~GameScene(void);
	Player* player;
	list<Background*> backList;
	list<Map*> mapList;
	list<Drop*> dropList;
	list<Building*> buildingList;
	list<Coin*> coinList;
	Drop *drop;
	Background *background;
	Map *map;
	Building *building;
	Coin *coin;
	Number *number1;
	Number *number2;
	Number *number3;
	Number *number4;
	void Render();
	void Update(float dTime);
	void InstBack();
	void InstDrop();
	void InstCoin();
	void InstBuilding();
	bool isGameOver;
	bool backDrow;
	bool dropDrow;
	bool first;
	bool up;
	float DropDelay;
	float DropDelayTimer;
	float CoinDelay;
	float CoinDelayTimer;
	float InstDelayTime;
	float InstDelayTimer;
	int Score;
	int upCoin;
	char str[4];
};

