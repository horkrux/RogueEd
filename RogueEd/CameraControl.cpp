#include "CameraControl.h"

CameraControl::CameraControl()
{
	m_frameTime = 0.0f;
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
	m_positionX = 0.0f;
	m_positionZ = -10.0f;
	m_position = XMFLOAT3(0, 0.f, 0);

	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_positionTerrainY = 0.0f;
}


CameraControl::CameraControl(const CameraControl& other)
{
}


CameraControl::~CameraControl()
{
}

void CameraControl::SetFrameTime(float time)
{
	m_frameTime = time;
}

void CameraControl::GetRotation(XMFLOAT3& rot)
{
	rot = m_rotation;
}

void CameraControl::GetRotationX(float& x)
{
	x = m_rotationX;
}

void CameraControl::GetPositionX(float& x)
{
	x = m_positionX;
}

void CameraControl::GetPositionZ(float& z)
{
	z = m_positionZ;
}

void CameraControl::GetPosition(XMFLOAT3& pos)
{
	pos = m_position;
}

void CameraControl::GetPositionTerrainY(float& y)
{
	y = m_positionTerrainY;
}

void CameraControl::Reset()
{
	m_position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void CameraControl::MoveLeft(bool keydown)
{
	if (keydown)
	{
		float radians = m_rotation.y * 0.0174532925f;

		m_position.x -= cosf(radians) * m_frameTime * 0.005f;
		m_position.z += sinf(radians) * m_frameTime * 0.005f;
	}
}

void CameraControl::MoveRight(bool keydown)
{
	if (keydown)
	{
		float radians = m_rotation.y * 0.0174532925f;

		m_position.x += cosf(radians) * m_frameTime * 0.005f;
		m_position.z -= sinf(radians) * m_frameTime * 0.005f;
	}
}

void CameraControl::MoveForwards(bool keydown)
{
	if (keydown)
	{
		float radiansY = m_rotation.y * 0.0174532925f;
		float radiansX = m_rotation.x * 0.0174532925f;

		m_position.x += sinf(radiansY) * cosf(radiansX) * m_frameTime * 0.005f;
		m_position.z += cosf(radiansY) * cosf(radiansX) * m_frameTime * 0.005f;
		m_position.y -= sinf(radiansX) * m_frameTime * 0.005f;
	}
}

void CameraControl::MoveBackwards(bool keydown)
{
	if (keydown)
	{
		float radiansY = m_rotation.y * 0.0174532925f;
		float radiansX = m_rotation.x * 0.0174532925f;

		m_position.x -= sinf(radiansY) * cosf(radiansX) * m_frameTime * 0.005f;
		m_position.z -= cosf(radiansY) * cosf(radiansX) * m_frameTime * 0.005f;
		m_position.y += sinf(radiansX) * m_frameTime * 0.005f;
	}
}

void CameraControl::TurnY(bool mouseRightClicked, long mouseX)
{
	if (mouseRightClicked)
	{
		m_rotation.y += mouseX * 0.1f;
	}
}

void CameraControl::TurnX(bool mouseRightClicked, long mouseY)
{
	if (mouseRightClicked)
	{
		m_rotation.x += mouseY * 0.1f;
	}
}

void CameraControl::AdjustTerrainHeight(bool mouseLeftClicked, long mouseY)
{
	if (mouseLeftClicked)
	{
		m_positionTerrainY += mouseY * 0.01f;
	}
}

void CameraControl::TurnLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 0.01f;

		if (m_leftTurnSpeed > (m_frameTime * 0.15f))
		{
			m_leftTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotation.y -= m_leftTurnSpeed;
	if (m_rotation.y < 0.0f)
	{
		m_rotation.y += 360.0f;
	}
}


void CameraControl::TurnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 0.01f;

		if (m_rightTurnSpeed > (m_frameTime * 0.15f))
		{
			m_rightTurnSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotation.y += m_rightTurnSpeed;
	if (m_rotation.y > 360.0f)
	{
		m_rotation.y -= 360.0f;
	}
}