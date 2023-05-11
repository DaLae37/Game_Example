#pragma once
#include "Common.h"
#include "Model.h"

class Character : public Model
{
public :
	enum ChracterDirection {
		UP,
		LEFT,
		DOWN,
		RIGHT
	};
	XMVECTOR m_currentPosition;
private :
	static constexpr const XMVECTORF32 DEFAULT_FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };
	static constexpr const XMVECTORF32 DEFAULT_RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };

	XMVECTOR m_targetPosition;
	FLOAT m_moveLeftRight;
	FLOAT m_moveBackForward;

	ChracterDirection m_currenRotation;
	float m_movementSpeed;
public :
	Character(const std::filesystem::path & filePath);
	~Character();

	void HandleInput(_In_ InputDirections directions, _In_ FLOAT deltaTime);
	void Update(_In_ FLOAT deltaTime)override;
};