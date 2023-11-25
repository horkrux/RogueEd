#include "DbgShape.h"

DbgShape::DbgShape()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_worldMatrix = XMMatrixIdentity();
	m_eventId = 0;
	m_color = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	m_indexCount = 0;
}

DbgShape::DbgShape(const DbgShape& cpy)
{

}

DbgShape::~DbgShape()
{
	/*if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}*/
}

//int DbgShape::read(char* pThingy)
//{
//	memcpy(&m_paramsBasic, pThingy, sizeof(ParamsBasic));
//
//	if (m_paramsBasic.offsetName)
//	{
//		m_name = pThingy + m_params.offsetName;
//	}
//
//	m_eventId = m_params.index;
//	m_position = m_params.position;
//	m_rotation = m_params.rotation;
//	m_scale = m_params.scale;
//
//	m_worldMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x * 0.0174532925f, m_rotation.y * 0.0174532925f, m_rotation.z * 0.0174532925f);
//	m_worldMatrix *= XMMatrixScaling(m_scale.x, -m_scale.y, m_scale.z);
//	m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);
//
//	return 0;
//}

XMFLOAT3 DbgShape::getPosition()
{
	return m_position;
}

void DbgShape::setPosition(XMFLOAT3 position)
{
	m_position = position;
}

XMVECTOR DbgShape::getColor()
{
	return m_color;
}

void DbgShape::setColor(XMVECTOR color)
{
	m_color = color;
}

int DbgShape::getIndexCount()
{
	return m_indexCount;
}

//XMFLOAT3 DbgShape::getRotation()
//{
//	return m_rotation;
//}
//
//void DbgShape::setRotation(XMFLOAT3 rotation)
//{
//	m_rotation = rotation;
//}
//
//XMFLOAT3 DbgShape::getScale()
//{
//	return m_scale;
//}
//
//void DbgShape::setScale(XMFLOAT3 scale)
//{
//	m_scale = scale;
//}

XMMATRIX DbgShape::getWorldMatrix()
{
	return m_worldMatrix;
}

void DbgShape::setWorldMatrix(XMMATRIX worldMatrix)
{
	m_worldMatrix = worldMatrix;
}

string DbgShape::getName()
{
	return m_name;
}

int DbgShape::getEventId()
{
	return m_eventId;
}

void DbgShape::setEventId(int id)
{
	m_eventId = id;
}

DbgShape::ShapeType DbgShape::getShapeType()
{
	return m_shapeType;
}

void DbgShape::render(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTypeDbgShape);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}