#pragma once

#include "DataIO.h"
#include "ModelCommonTypes.h"

#include <d3d11_1.h>
//using namespace DirectX;


class DbgShape
{
public: 

	enum ShapeType
	{
		BOX,
		SPHERE
	};

public:
	DbgShape();
	DbgShape(const DbgShape&);
	virtual ~DbgShape() = 0;

	virtual int read(char*) = 0;

	XMFLOAT3 getPosition();
	void setPosition(XMFLOAT3);
	XMVECTOR getColor();
	void setColor(XMVECTOR);
	//XMFLOAT3 getRotation();
	//void setRotation(XMFLOAT3);
	//XMFLOAT3 getScale();
	//void setScale(XMFLOAT3);
	XMMATRIX getWorldMatrix();
	void setWorldMatrix(XMMATRIX);

	string getName();
	int getEventId();
	void setEventId(int);
	int getIndexCount();
	ShapeType getShapeType();

	void render(ID3D11Device1*, ID3D11DeviceContext1*);

protected:
	typedef struct {
		uint32_t type;
		uint32_t index;
		uint32_t size;
		uint32_t offsetName;
		XMFLOAT3 position;
		//XMFLOAT3 rotation;
		//XMFLOAT3 scale;
	} ParamsBasic;

	ShapeType m_shapeType;

	string m_name;
	XMVECTOR m_color;

	XMFLOAT3 m_position;
	//XMFLOAT3 m_rotation;
	//XMFLOAT3 m_scale;

	int m_indexCount;

	XMMATRIX m_worldMatrix;

	int m_eventId;

	ParamsBasic m_paramsBasic;

	ID3D11Buffer* vertexBuffer, * indexBuffer;

};