#include "Character.h"


Character::Character(const std::filesystem::path& filePath) : Model(filePath) {
	m_movementSpeed = 10;
	m_targetPosition = XMVECTOR{ 0, 0, 0};
	m_currentPosition = XMVECTOR{ 0, 0, 0};
	m_moveLeftRight = 0;
	m_moveBackForward = 0;

	m_currenRotation = DOWN;
}

Character::~Character() {

}

void Character::HandleInput(_In_ const InputDirections directions, _In_ FLOAT deltaTime) {

	ChracterDirection before = m_currenRotation;
	m_moveBackForward = 0;
	m_moveLeftRight = 0;
	if (directions.bFront)
	{
		m_currenRotation = UP;
		m_moveBackForward += deltaTime * m_movementSpeed;
	}
	else if (directions.bLeft)
	{
		m_currenRotation = LEFT;
		m_moveLeftRight -= deltaTime * m_movementSpeed;
	}
	else if (directions.bBack)
	{
		m_currenRotation = DOWN;
		m_moveBackForward -= deltaTime * m_movementSpeed;
	}
	else if (directions.bRight)
	{
		m_currenRotation = RIGHT;
		m_moveLeftRight += deltaTime * m_movementSpeed;
	}
	float degree = (before - m_currenRotation) * 90;
	if (degree != 0) {
		//Renderable::RotateYInObjectCoordinate(degree, m_currentPosition);
	}
}

void Character::Update(_In_ FLOAT deltaTime) {
	Model::Update(deltaTime);

	m_targetPosition = m_currentPosition;

	m_targetPosition += DEFAULT_FORWARD * m_moveBackForward;
	m_targetPosition += DEFAULT_RIGHT * m_moveLeftRight;
	XMVECTOR offset = m_targetPosition - m_currentPosition;
	Renderable::Translate(offset);
	m_currentPosition = m_targetPosition;
}