#include "RogueObject.h"



RogueObject::RogueObject()
{
	m_type = DEFAULT;
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_worldMatrix = XMMatrixIdentity();
	m_health = 0;
	m_modelId = 0;
	m_eventId = 0;
	//m_hobReference = NULL;
}

RogueObject::RogueObject(const RogueObject& cpy)
{

}

RogueObject::~RogueObject()
{

}

//RogueObjectType RogueObject::getType()
//{
//	return m_type;
//}

//void RogueObject::setType(RogueObjectType type)
//{
//	m_type = type;
//}

XMFLOAT3 RogueObject::getPosition()
{
	return m_position;
}

void RogueObject::setPosition(XMFLOAT3 position)
{
	m_position = position;
}

XMFLOAT3 RogueObject::getRotation()
{
	return m_rotation;
}

void RogueObject::setRotation(XMFLOAT3 rotation)
{
	m_rotation = rotation;
}

XMFLOAT3 RogueObject::getScale()
{
	return m_scale;
}

void RogueObject::setScale(XMFLOAT3 scale)
{
	m_scale = scale;
}

XMMATRIX RogueObject::getWorldMatrix()
{
	return m_worldMatrix;
}

void RogueObject::setWorldMatrix(XMMATRIX worldMatrix)
{
	m_worldMatrix = worldMatrix;
}

unsigned int RogueObject::getHealth()
{
	return m_health;
}

void RogueObject::setHealth(unsigned int health)
{
	m_health = health;
}

int RogueObject::getModelId()
{
	return m_modelId;
}

void RogueObject::setModelId(int modelId)
{
	m_modelId = modelId;
}

int RogueObject::getEventId()
{
	return m_eventId;
}

void RogueObject::setEventId(int id)
{
	m_eventId = id;
}

//HOB* RogueObject::getHobReference()
//{
//	return m_hobReference;
//}
//
//void RogueObject::setHobReference(HOB* hob)
//{
//	m_hobReference = hob;
//}