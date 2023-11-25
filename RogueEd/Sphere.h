#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(ID3D11Device1*, ID3D11DeviceContext1*);
	Sphere(const Sphere&);

	void addReference();
	int removeReference();

private:
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	static int m_referenceCount;
};