#include "DbgShapeBox.h"

Box* DbgShapeBox::m_referenceBox = NULL;
//int DbgShapeBox::referenceCount = 0; //MOVE THIS DOWN TO BOX CLASS

DbgShapeBox::DbgShapeBox(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
	//vertexBuffer = 0;
	//indexBuffer = 0;
	//D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	//D3D11_SUBRESOURCE_DATA vertexData, indexData;
	//VertexTypeDbgShape vertices[8];
	//vector<uint16_t> indices = { 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4 }; //a box

	//HRESULT result;
	//vertices[0] = VertexTypeDbgShape(0, 0, 0);
	//vertices[1] = VertexTypeDbgShape(1, 0, 0);
	//vertices[2] = VertexTypeDbgShape(1, 0, 1);
	//vertices[3] = VertexTypeDbgShape(0, 0, 1);
	//vertices[4] = VertexTypeDbgShape(0, 1, 0);
	//vertices[5] = VertexTypeDbgShape(1, 1, 0);
	//vertices[6] = VertexTypeDbgShape(1, 1, 1);
	//vertices[7] = VertexTypeDbgShape(0, 1, 1);

	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(VertexTypeDbgShape) * 8;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	//vertexBufferDesc.StructureByteStride = 0;

	//vertexData.pSysMem = vertices;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;

	//result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = 2 * indices.size();
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//indexData.pSysMem = indices.data();
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	if (!m_referenceBox)
	{
		m_referenceBox = new Box(device, deviceContext);
	}

	m_referenceBox->addReference();

	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_indexCount = 24;

	m_shapeType = DbgShape::ShapeType::BOX;
	
}

DbgShapeBox::DbgShapeBox(const DbgShapeBox& cpy) {}

DbgShapeBox::~DbgShapeBox()
{
	if (!m_referenceBox->removeReference())
		m_referenceBox = NULL;
}

int DbgShapeBox::read(char* pThingy)
{
	memcpy(&m_params, pThingy, sizeof(Params));

	if (m_params.offsetName)
	{
		m_name = pThingy + m_params.offsetName;
	}

	//set color based on "type"
	switch (m_params.type)
	{
	case 0x60016:
		m_color = XMVectorSet(1.f, 0, 0, 1.f);
		break;
	case 0x60004:
		m_color = XMVectorSet(0.f, 1.f, 0, 1.f);
		break;
	case 0x60005:
		m_color = XMVectorSet(0.f, 1.f, 1.f, 1.f);
		break;
	case 0x60010:
		m_color = XMVectorSet(1.f, 0.f, 1, 1.f);
		break;
	case 0x60039:
		m_color = XMVectorSet(1.f, 1.f, 0, 1.f);
		break;
	}
	

	m_eventId = m_params.index;
	m_position = m_params.position;
	m_rotation = m_params.rotation;
	m_scale = m_params.scale;

	m_worldMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x * 0.0174532925f, m_rotation.y * 0.0174532925f, m_rotation.z * 0.0174532925f);
	m_worldMatrix *= XMMatrixScaling(m_scale.x, -m_scale.y, m_scale.z);
	m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);

	return 0;
}

XMFLOAT3 DbgShapeBox::getRotation()
{
	return m_rotation;
}

void DbgShapeBox::setRotation(XMFLOAT3 rotation)
{
	m_rotation = rotation;
}

XMFLOAT3 DbgShapeBox::getScale()
{
	return m_scale;
}

void DbgShapeBox::setScale(XMFLOAT3 scale)
{
	m_scale = scale;
}

Box* DbgShapeBox::getReferenceBox()
{
	return m_referenceBox;
}