#include "IObject.h"
#include"Global.h"

IObject::IObject(void)
{
	runspeed = 550;
	pos= Vec2(0,0);
	rotationCenter= Vec2(0,0);
	scalingCenter= Vec2(0,0);
	scale= Vec2(1,1);
	rot= 0;

	D3DXMatrixIdentity(&mat);
	
	rect.top= 0;
	rect.left= 0;
	rect.right= 0;
	rect.bottom= 0;

	parent= nullptr;
	
}


IObject::~IObject(void)
{
	for(auto& child:childList) {
		delete child;
	}
}

void:: IObject :: Render() {
	D3DXMatrixTransformation2D(&mat,&scalingCenter,0,&scale,&rotationCenter,rot,&pos);
	if(parent != nullptr) {
		mat *= parent->mat;
	}

}

void::IObject::Update(float dTime) {

	for(auto& child:childList) {
		child->Update(dTime);

	}

}

void IObject::AddChild(IObject*child) {
	childList.push_back(child);
	child->parent = this;

}

void IObject::RemoveChild(IObject*child) {
	childList.remove(child);
	child->parent= nullptr;

}

void IObject::SetCenter(int width, int height, IObject* sprite)
{
	rect.left = -width / 2;
	rect.top = -height / 2;
	rect.right = width / 2;
	rect.bottom = height / 2;

	sprite->pos = Vec2(-width / 2, -height/2);
}