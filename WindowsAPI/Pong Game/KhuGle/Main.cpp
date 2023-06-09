//
//	Dept. Software Convergence, Kyung Hee University
//	Prof. Daeho Lee, nize@khu.ac.kr
//
#include "KhuGleWin.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "Brick.h"
#pragma warning(disable:4996)

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class CCollision : public CKhuGleWin
{
private :
	bool isStart;
	short HP;
	int remainBricks;
	bool isCollision;

public:
	CKhuGleSprite* player;
	CKhuGleSprite *ball;
	CKhuGleSprite *maps[4]; //TOP, RIGHT, BOTTOM, LEFT 벽
	std::vector<Brick *>bricks; // 5 x 10

	void CreateLevel();
	void CollisionCheck();
	void InGameInputCheck();
	void UIRender();


	CCollision(int nW, int nH);
	void Update();
	CKhuGleLayer* m_pGameLayer;
	CKgPoint m_LButtonStart, m_LButtonEnd;
	int m_nLButtonStatus;

	CKgVector2D GetProjectionResult(CKhuGleSprite* s1);
	double GetLineCollisionResult(CKhuGleSprite* s1);
	bool GetAABBCollisionResult(CKhuGleSprite* s1);
	void MakeVritualCricleToCollision(CKhuGleSprite* s1);
};

CCollision::CCollision(int nW, int nH) : CKhuGleWin(nW, nH) 
{
	isStart = false;
	HP = 3;
	isCollision = false;

	m_nLButtonStatus = 0;

	m_Gravity = CKgVector2D(0., 98.);
	m_AirResistance = CKgVector2D(0.1, 0.1);

	m_pScene = new CKhuGleScene(640, 480, KG_COLOR_24_RGB(100, 100, 150));

	m_pGameLayer = new CKhuGleLayer(600, 420, KG_COLOR_24_RGB(150, 150, 200), CKgPoint(20, 30));

	m_pScene->AddChild(m_pGameLayer);

	CreateLevel();
}

void CCollision::CreateLevel() { //레벨 초기화
	if (m_pGameLayer->m_Children.size() != 0) {
		ball->MoveTo(270 - ball->m_Radius, 270 - ball->m_Radius);
		ball->m_Velocity = CKgVector2D(150, 100);
		player->MoveTo(300, 405);
	}
	else{//처음 레벨을 생성했을 때
		ball = new CKhuGleSprite(GP_STYPE_ELLIPSE, GP_CTYPE_DYNAMIC, CKgLine(CKgPoint(270, 270), CKgPoint(300, 300)),
			KG_COLOR_24_RGB(255, 0, 0), true, 100);
		ball->m_Velocity = CKgVector2D(150, 100);
		m_pGameLayer->AddChild(ball);

		player = new CKhuGleSprite(GP_STYPE_RECT, GP_CTYPE_KINEMATIC, CKgLine(CKgPoint(250, 390), CKgPoint(350, 420)),
			KG_COLOR_24_RGB(255, 255, 255), true, 0);
		m_pGameLayer->AddChild(player);
		player->m_Velocity = CKgVector2D(200, 0);

		int currentBricks = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 10; j++) {
				int r = rand() % 100 + 1;
				if (r < 75) {
					Brick* b = new Brick(GP_STYPE_RECT, GP_CTYPE_KINEMATIC, CKgLine(CKgPoint(j * 60, i * 30), CKgPoint(j * 60 + 60, i * 30 + 30)),
						KG_COLOR_24_RGB(0, 255, 255), false, 60);
					currentBricks++;
					bricks.push_back(b);
					m_pGameLayer->AddChild(b);
				}
			}
		}
		remainBricks = currentBricks;

		maps[0] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_KINEMATIC, CKgLine(CKgPoint(0, 0), CKgPoint(600, 0)),
			KG_COLOR_24_RGB(255, 255, 0), false, 0);
		maps[1] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_KINEMATIC, CKgLine(CKgPoint(599, 0), CKgPoint(599, 420)),
			KG_COLOR_24_RGB(255, 255, 0), false, 0);
		maps[2] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_STATIC, CKgLine(CKgPoint(0, 419), CKgPoint(599, 419)),
			KG_COLOR_24_RGB(255, 0, 0), false, 0);
		maps[3] = new CKhuGleSprite(GP_STYPE_LINE, GP_CTYPE_KINEMATIC, CKgLine(CKgPoint(0, 0), CKgPoint(0, 420)),
			KG_COLOR_24_RGB(255, 255, 0), false, 0);

		for (int i = 0; i < 4; i++) {
			m_pGameLayer->AddChild(maps[i]);
		}
	}
}


void CCollision::InGameInputCheck() {
	if (m_bKeyPressed[VK_LEFT] && player->m_rtBoundBox.Left > 0) { //왼쪽 화면을 넘어갈 경우 움직임 통제
		player->MoveBy(-player->m_Velocity.x * m_ElapsedTime, 0);
	}
	if (m_bKeyPressed[VK_RIGHT] && player->m_rtBoundBox.Right < 600) { //오른쪽 화면을 넘어갈 경우 움직임 통제
		player->MoveBy(player->m_Velocity.x * m_ElapsedTime, 0);
	}
}

CKgVector2D CCollision::GetProjectionResult(CKhuGleSprite* s1) { //Projection Vector
	CKgVector2D line = CKgVector2D(s1->m_lnLine.End.X - s1->m_lnLine.Start.X,
		s1->m_lnLine.End.Y - s1->m_lnLine.Start.Y);
	CKgVector2D lineTball = CKgVector2D(ball->m_Center.x - s1->m_lnLine.Start.X,
		ball->m_Center.y - s1->m_lnLine.Start.Y);

	double v11 = line.Dot(line);
	double projectionRate = max(0, min(v11, line.Dot(lineTball))) / v11;

	CKgVector2D projection = CKgVector2D(s1->m_lnLine.Start) + projectionRate * line;
	return projection;
}

double CCollision::GetLineCollisionResult(CKhuGleSprite *s1) { //공과 다른 선 객체와의 충돌 확인
	CKgVector2D normal = ball->m_Center - GetProjectionResult(s1);

	double overlapped = CKgVector2D::abs(normal) - ball->m_Radius;

	return overlapped;
}

bool CCollision::GetAABBCollisionResult(CKhuGleSprite* s1) { //공과 다른 사각형 객체와의 충돌 확인
	return (s1->m_rtBoundBox.Left <= ball->m_Center.x + ball->m_Radius) &&
		(s1->m_rtBoundBox.Right >= ball->m_Center.x - ball->m_Radius) &&
		(s1->m_rtBoundBox.Top <= ball->m_Center.y + ball->m_Radius) &&
		(s1->m_rtBoundBox.Bottom >= ball->m_Center.y - ball->m_Radius);
}

void CCollision::MakeVritualCricleToCollision(CKhuGleSprite* s1) { //가상의 원을 만들어 공과 충돌처리
	CKgVector2D project = GetProjectionResult(s1);
	CKgVector2D PosVec = project - ball->m_Center;
	double posVecY = PosVec.y;
	double posVecX = PosVec.x;
	double Distance = CKgVector2D::abs(PosVec);
	if (Distance == 0) Distance = 1E-6;
	CKgVector2D Normal = (1. / Distance) * PosVec;

	double kx = (ball->m_Velocity.x - s1->m_Velocity.x);
	double ky = (ball->m_Velocity.y - s1->m_Velocity.y);
	double p = (Normal.x * kx + Normal.y * ky) / ball->m_Mass;

	ball->m_Velocity.x = (ball->m_Velocity.x - p * Normal.x);
	ball->m_Velocity.y = (ball->m_Velocity.y - p * Normal.y);

	if (posVecY * ball->m_Velocity.y >= 0) {
		ball->m_Velocity.y *= -1;
	}
	else if (posVecX * ball->m_Velocity.x >= 0 && s1->m_Velocity.x == 0) {
		ball->m_Velocity.x *= -1;
	}
}

void CCollision::CollisionCheck() {
	if (player->m_rtBoundBox.Left < 0) { //왼쪽 화면을 넘어가지 못하게 값 조정
		player->m_rtBoundBox.Left = 0;
	}
	if (player->m_rtBoundBox.Right > 600) {//오른쪽 화면을 넘어가지 못하게 값 조정
		player->m_rtBoundBox.Right = 600;
	}

	for (int i = 0; i < 4; i++) { //벽과의 충돌

		double overlapped = GetLineCollisionResult(maps[i]);

		if (overlapped <= 0) {
			switch (i)
			{
			case 0:
				ball->m_Velocity.x = ball->m_Velocity.x;
				ball->m_Velocity.y = -ball->m_Velocity.y;
				break;
			case 1:
				ball->m_Velocity.x = -ball->m_Velocity.x;
				break;
			case 2:
				HP -= 1;
				CreateLevel();
				return;
			case 3:
				ball->m_Velocity.x = -ball->m_Velocity.x;
				break;
			default:
				break;
			}
		}
	}

	isCollision = false;
	for (Brick* brick : bricks) { //벽돌과 충돌
		if (!brick->getIsBroken()) {

			bool overlapped = GetAABBCollisionResult(brick);

			if (overlapped) {
				if (!isCollision) {
					MakeVritualCricleToCollision(brick);
					isCollision = true;
				}
				brick->setBrickHP(brick->getBrickHP() - 1);
			}

			if (brick->getBrickHP() <= 0) {
				brick->setIsBroken(true);
				remainBricks -= 1;
			}
		}
	}

	bool overlapped = GetAABBCollisionResult(player); //바닥 블록과 충돌

	if (overlapped) {
		MakeVritualCricleToCollision(player);
	}
}

void CCollision::UIRender() {	//UI
	DrawSceneTextPos("Pong Game", CKgPoint(0, 0));
	DrawSceneTextPos("남은 벽돌 : ", CKgPoint(300, 0));
	DrawSceneTextPos(std::to_string(remainBricks).c_str(), CKgPoint(400, 0));
	DrawSceneTextPos("HP : ", CKgPoint(550, 0));
	DrawSceneTextPos(std::to_string(HP).c_str(), CKgPoint(600, 0));
}

void CCollision::Update()
{
	m_pScene->Render();
	CKhuGleWin::Update();

	//UI
	UIRender();

	if (HP > 0) {
		if (isStart) {
			if (remainBricks <= 0) { //모든 블록을 제거 시
				DrawSceneTextPos("Clear! (Press ESC to Restart)", CKgPoint(m_pScene->m_nH / 2 - 50, m_pScene->m_nH / 2));
				if (m_bKeyPressed[VK_ESCAPE]) {
					isStart = false;
					HP = 3;
					CreateLevel();
				}
			}
			else {
				ball->MoveBy(ball->m_Velocity.x * m_ElapsedTime, ball->m_Velocity.y * m_ElapsedTime);
				InGameInputCheck();
				CollisionCheck();
			}
		}
		else { //스페이스바 입력 시 게임 시작
			DrawSceneTextPos("Press Space to Start", CKgPoint(m_pScene->m_nH / 2 - 25, m_pScene->m_nH / 2));
			if (m_bKeyPressed[VK_SPACE]) {
				isStart = true;
			}
		}
	}
	else { // HP가 0이 되었을 떄
		DrawSceneTextPos("Game Over! (Press ESC to Exit)", CKgPoint(m_pScene->m_nH / 2 - 50, m_pScene->m_nH / 2));
		if (m_bKeyPressed[VK_ESCAPE]) {
			PostQuitMessage(0);
		}
	}
}

int main()
{
	srand(time(NULL));
	CCollision *pCollision = new CCollision(640, 480);

	KhuGleWinInit(pCollision);

	return 0;
}