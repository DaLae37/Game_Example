#include "HowScene.h"
#include "Global.h"
#include "MainScene.h"
HowScene::HowScene()
{
	howscene = new Sprite("howscene.png");
	AddObject(howscene);
	howscene->pos = Vec2(0, 0);
	howout = new Sprite("how-out.png");
	AddObject(howout);
	howout->pos = Vec2(900, 520);
}


HowScene::~HowScene()
{
}

void HowScene::Render()
{
	howscene->Render();
	howout->Render();
}

void HowScene::Update(float dTime)
{
	Scene::Update(dTime);
	if (GetMyKeyState(VK_LBUTTON) == 1)
	{
		if (IsPointInRect(howout->pos, howout->rect, GetMousePos()))
		{
			sceneManager->ChangeScene(new MainScene());
		}
	}
}