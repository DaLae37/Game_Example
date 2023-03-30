#pragma once
#include<d3dx9.h>
#include<list>

using namespace std;

class IObject
{
public:
	IObject(void);
	virtual ~IObject(void);
	int runspeed;
	IObject* parent;

	D3DXMATRIX mat;
	D3DXVECTOR2 scalingCenter;
	D3DXVECTOR2 scale;
	D3DXVECTOR2 rotationCenter;
	float rot;
	D3DXVECTOR2 pos;

	RECT rect;

	list<IObject*> childList;

	void AddChild(IObject*child);
	void RemoveChild(IObject* child);

	virtual void Render();
	virtual void Update(float dTime);

	void SetCenter(int width, int height, IObject *sprite);
};

