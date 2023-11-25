#ifndef _ENTITYCONTROL_H_
#define _ENTITYCONTROL_H_

#include <math.h>
#include <directxmath.h>
using namespace DirectX;

class CameraControl
{
public:
	CameraControl();
	CameraControl(const CameraControl&);
	~CameraControl();

	void SetFrameTime(float);
	void GetRotation(XMFLOAT3&);
	void GetRotationX(float&);
	void GetPositionX(float&);
	void GetPositionZ(float&);
	void GetPosition(XMFLOAT3&);
	void GetPositionTerrainY(float&);

	void Reset();

	void MoveRight(bool);
	void MoveLeft(bool);
	void MoveForwards(bool);
	void MoveBackwards(bool);
	void TurnY(bool, long);
	void TurnX(bool, long);
	void TurnLeft(bool);
	void TurnRight(bool);
	void AdjustTerrainHeight(bool, long);

private:
	float m_frameTime;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_position;
	float m_rotationX;
	float m_rotationY;
	float m_positionX;
	float m_positionZ;
	float m_leftTurnSpeed, m_rightTurnSpeed;

	float m_positionTerrainY;
};

#endif