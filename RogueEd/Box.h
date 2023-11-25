#pragma once

#include "Shape.h"

class Box : public Shape
{
public:
	Box(ID3D11Device1*, ID3D11DeviceContext1*);
	Box(const Box&);

	void addReference();
	int removeReference();

private:
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	static int m_referenceCount;
};