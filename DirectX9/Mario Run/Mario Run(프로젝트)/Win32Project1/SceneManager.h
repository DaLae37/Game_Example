#pragma once
#include"Scene.h"
class SceneManager
{
public:
	SceneManager(void);
	~SceneManager(void);
	Scene*currentScene;

	void Render();
	void Update(float dTime);

	void ChangeScene(Scene*scene);

};

