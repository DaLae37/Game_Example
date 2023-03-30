#include "Animation.h"


Animation::Animation(int fps)
{
	this->fps = fps;
	maxFrame=0;
	currentFrame = 0;
	timeChecker= 0;

	height = 0;
	width = 0;

	color= D3DCOLOR_ARGB(255,255,255,255);
}


Animation::~Animation(void)
{
	
}

void Animation::Render() {
	IObject::Render();
	if(maxFrame> 0) {
		spriteVector[currentFrame]->color = color;
		spriteVector[currentFrame]->Render();
	}
}

void Animation::Update(float dTime) {
	timeChecker+=dTime;
	if(timeChecker>1.f/fps) {
		timeChecker=0;
		currentFrame++;
		if(currentFrame==maxFrame) {
			currentFrame=0;
		}
	}
}

void Animation::AddFrame(char* path) {
	spriteVector.push_back(new Sprite(path));
	AddChild(spriteVector[maxFrame]);
	if (maxFrame == 0)
	{
		width = spriteVector[maxFrame]->width;
		height = spriteVector[maxFrame]->height;
	}
	maxFrame++;
}