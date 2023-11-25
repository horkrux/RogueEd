#include "Box.h"

int Box::m_referenceCount = 0;

Box::Box(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	VertexTypeDbgShape vertices[8];
	vector<uint16_t> indices = { 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4 }; //a box

	HRESULT result;
	vertices[0] = VertexTypeDbgShape(0, 0, 0);
	vertices[1] = VertexTypeDbgShape(1, 0, 0);
	vertices[2] = VertexTypeDbgShape(1, 0, 1);
	vertices[3] = VertexTypeDbgShape(0, 0, 1);
	vertices[4] = VertexTypeDbgShape(0, 1, 0);
	vertices[5] = VertexTypeDbgShape(1, 1, 0);
	vertices[6] = VertexTypeDbgShape(1, 1, 1);
	vertices[7] = VertexTypeDbgShape(0, 1, 1);

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeDbgShape) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = 2 * indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	//m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_indexCount = 24;
}

Box::Box(const Box&) {}

void Box::addReference()
{
	m_referenceCount++;
}

int Box::removeReference()
{
	if (!--m_referenceCount)
	{
		delete this;
	}

	return m_referenceCount;
}

//void Box::Render(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
//{
//	unsigned int stride;
//	unsigned int offset;
//
//
//	// Set vertex buffer stride and offset.
//	stride = sizeof(VertexTypeDbgShape);
//	offset = 0;
//
//	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
//	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//}
