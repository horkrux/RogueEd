#pragma once

#include <cmath>

#include "DbgShape.h"



class Spline : public DbgShape
{
public:
	Spline(ID3D11Device1*, ID3D11DeviceContext1*);
	Spline(const Spline&);
	~Spline();
	void CalcSpline(ID3D11Device1*, ID3D11DeviceContext1*);

	virtual int read(char*);

private:
	float GetT(float, float, const XMVECTOR&, const XMVECTOR&);
	float lerp(float, float, float);
	XMVECTOR CatMullRom(const XMVECTOR&, const XMVECTOR&, const XMVECTOR&, const XMVECTOR&, float, float alpha = .5f);

protected:
	typedef struct params : ParamsBasic
	{
		uint32_t numPoints;
		uint32_t offsetPoints;
		float sthSpeedRelated;
		uint32_t numEventPoints;
		uint32_t offsetEventPointInfo; //idk what the values mean
		uint32_t unk;
	} Params;

	Params m_params;

	XMVECTOR* m_points;
	vector<XMVECTOR> m_splinePoints;
};