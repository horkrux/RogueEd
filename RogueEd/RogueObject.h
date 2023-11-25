#ifndef _ROGUEOBJECT_H_
#define _ROGUEOBJECT_H_

#include "RogueObjectType.h"
#include "HOB.h"
#include "DataIO.h"
#include <d3d11_1.h>

#include <directxmath.h>
using namespace DirectX;


class RogueObject
{
public:
	RogueObject();
	RogueObject(const RogueObject&);
	virtual ~RogueObject() = 0;

	virtual RogueObjectType getType() const = 0;
	//void setType(RogueObjectType);

	virtual int read(char*) = 0;

	XMFLOAT3 getPosition();
	void setPosition(XMFLOAT3);
	XMFLOAT3 getRotation();
	void setRotation(XMFLOAT3);
	XMFLOAT3 getScale();
	void setScale(XMFLOAT3);
	XMMATRIX getWorldMatrix();
	void setWorldMatrix(XMMATRIX);

	unsigned int getHealth();
	void setHealth(unsigned int);
	int getModelId();
	void setModelId(int);
	int getEventId();
	void setEventId(int);
	virtual HOB* getHobReference() = 0;
	virtual void setHobReference(HOB*) = 0;

protected:
	RogueObjectType m_type;
	string m_name;

	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	XMMATRIX m_worldMatrix;

	unsigned int m_health;
	int m_modelId;
	int m_eventId;

	//static HOB* m_hobReference;
};

#endif