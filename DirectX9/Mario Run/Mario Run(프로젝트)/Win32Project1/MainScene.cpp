#include "MainScene.h"
#include "Global.h"
#include "GameScene.h"
#include "HowScene.h"
MainScene::MainScene()
{
	name = new Sprite("mainname.png");
	AddObject(name);
	name->pos = Vec2(275, 50);
	mainBack = new Sprite("Background.png");
	AddObject(mainBack);
	mainBack->pos = Vec2(0, 0);
	startButton = new Sprite("in.png");
	AddObject(startButton);
	startButton->pos = Vec2(SCREEN_WIDTH / 2-startButton->width/2, 350);
	exitButton = new Sprite("out.png");
	AddObject(exitButton);
	exitButton->pos = Vec2(SCREEN_WIDTH / 2-exitButton->width/2, 480);
	how = new Sprite("how.png");
	AddObject(how);
	how->pos = Vec2(900, 520);
}


MainScene::~MainScene()
{
	
}

void MainScene::Render()
{
	mainBack->Render();
	name->Render();
	startButton->Render();
	exitButton->Render();
	how->Render();
}

void MainScene::Update(float dTime)
{
	Scene::Update(dTime);
	if (GetMyKeyState(VK_LBUTTON) == 1)
	{
		if (IsPointInRect(startButton->pos, startButton->rect, GetMousePos()))
		{
			sceneManager->ChangeScene(new GameScene());
		}
		else if (IsPointInRect(exitButton->pos, exitButton->rect, GetMousePos()))
		{
			PostQuitMessage(0);
		}
		else if (IsPointInRect(how->pos, how->rect, GetMousePos()))
		{
			sceneManager->ChangeScene(new HowScene());
		}
		return;
	}

}