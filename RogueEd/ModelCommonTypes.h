#pragma once

#include <directxmath.h>
using namespace DirectX;

typedef struct
{
	int faceGroupCount;
	int* indexCounts;
	int* vertexCounts;
	int numMaterials;
	bool* matHasTex;
	int** SHITFUCK;
	int* SHITFUCKTWO;
} DrawInfo;

typedef struct vertexTypeObject
{
	XMVECTOR position;
	XMVECTOR color;
	XMFLOAT2 texUV;
	uint32_t shit;
	float padding1 = 0;
	vertexTypeObject() {};
	vertexTypeObject(float x, float y, float z, float colorR, float colorG, float colorB)
	{
		position = XMVectorSet(x, y, z, 1.f);
		color = XMVectorSet(colorR, colorG, colorB, 1);
		texUV = XMFLOAT2(0, 0);
		shit = 0;
		padding1 = 0;
	};
} VertexTypeObject;

typedef struct vertexTypeTerrain
{
	XMVECTOR position;
	XMVECTOR color;
	XMVECTOR normal;
	XMFLOAT2 texUV;
	float padding[2];
	vertexTypeTerrain() {};
	vertexTypeTerrain(float x, float y, float z, float nx, float ny, float nz)
	{
		position = XMVectorSet(x, y, z, 1.f);
		normal = XMVectorSet(nx, ny, nz, 1.f);
		color = XMVectorSet(1.f, 1.f, 1.f, 1.f);
		texUV = XMFLOAT2(0, 0);
		padding[0] = 0;
		padding[1] = 0;
	}
} VertexTypeTerrain;

typedef struct vertexTypeDbgShape
{
	XMVECTOR position;
	vertexTypeDbgShape() {};
	vertexTypeDbgShape(float x, float y, float z)
	{
		position = XMVectorSet(x, y, z, 1.f);
	}
} VertexTypeDbgShape;