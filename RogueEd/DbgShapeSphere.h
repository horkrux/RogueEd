#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

#include "DbgShape.h"
#include "Sphere.h"

class DbgShapeSphere : public DbgShape
{
public:
	DbgShapeSphere(ID3D11Device1*, ID3D11DeviceContext1*);
	DbgShapeSphere(const DbgShapeSphere&);
	~DbgShapeSphere();

	float getRadius();
	void setRadius(float);

	Sphere* getReferenceSphere();

	int whatever(char*, XMFLOAT3);
	virtual int read(char*);

protected:
	typedef struct params : ParamsBasic
	{
		float radius;
	} Params;

	Params m_params; //this is useless and I need to remove it

	float m_radius;

private:
	static Sphere* m_referenceSphere;
};

