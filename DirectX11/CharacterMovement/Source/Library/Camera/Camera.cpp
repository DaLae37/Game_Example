#include "Camera/Camera.h"

Camera::Camera(_In_ const XMVECTOR& position)
	:m_cbChangeOnCameraMovement()
    , m_yaw(0.0f)
    , m_pitch(0.0f)
    , m_moveLeftRight(0.0f)
    , m_moveBackForward(0.0f)
    , m_moveUpDown(0.0f)
    , m_movementSpeed(10.0f)
    , m_rotationSpeed(0.002f)
    , m_cameraForward(DEFAULT_FORWARD)
    , m_cameraRight(DEFAULT_RIGHT)
    , m_cameraUp(DEFAULT_UP)
    , m_eye(position)
    , m_at(position + m_cameraForward)
    , m_up(m_cameraUp)
    , m_rotation()
    , m_view(XMMatrixLookAtLH(m_eye, m_at, m_up))
{
}

const XMVECTOR& Camera::GetEye() const
{
    return m_eye;
}

const XMVECTOR& Camera::GetAt() const
{
    return m_at;
}

const XMVECTOR& Camera::GetUp() const
{
    return m_up;
}

const XMMATRIX& Camera::GetView() const
{
    return m_view;
}

ComPtr<ID3D11Buffer>& Camera::GetConstantBuffer()
{
    return m_cbChangeOnCameraMovement;
}

void Camera::HandleInput(_In_ const InputDirections& directions, _In_ const MouseRelativeMovement& mouseRelativeMovement, const BOOL& mouseRightClick, _In_ FLOAT deltaTime)
{
    if (directions.bFront)
    {
        m_moveBackForward += m_movementSpeed * deltaTime;
    }

    else if (directions.bLeft)
    {
        m_moveLeftRight -= m_movementSpeed * deltaTime;
    }

    else if (directions.bBack)
    {
        m_moveBackForward -= m_movementSpeed * deltaTime;
    }

    else if (directions.bRight)
    {
        m_moveLeftRight += m_movementSpeed * deltaTime;
    }
}

HRESULT Camera::Initialize(_In_  ID3D11Device* pDevice, _In_ ID3D11DeviceContext* pImmediateContext)
{
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBChangeOnCameraMovement);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pDevice->CreateBuffer(&bd, nullptr, m_cbChangeOnCameraMovement.GetAddressOf());
    if (FAILED(hr))
        return hr;

    return hr;
}

void Camera::Update(_In_ FLOAT deltaTime)
{
    m_eye += m_moveLeftRight * m_cameraRight;
    m_eye += m_moveBackForward * m_cameraForward;
    m_moveLeftRight = 0.0f;
    m_moveBackForward = 0.0f;

    m_at = m_eye;

    m_view = XMMatrixLookAtLH(m_eye + DEFAULT_TARGET * -10, m_at, DEFAULT_UP);
}

void Camera::SetCharacter(_In_ const std::shared_ptr<Character>& character) {
    m_character = character;
}