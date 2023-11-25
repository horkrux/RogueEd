#pragma once

#include <vector>
#include <d3d11_1.h>

#include "ModelCommonTypes.h"

using namespace std;

class Shape
{
public:
	Shape();
	Shape(const Shape&);
	~Shape();

	virtual void addReference() = 0;
	virtual int removeReference() = 0;

	int getIndexCount();
	void Render(ID3D11Device1*, ID3D11DeviceContext1*);

protected:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_indexCount;
};