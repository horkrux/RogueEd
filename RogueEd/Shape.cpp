#include "Shape.h"

Shape::Shape()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_indexCount = 0;
}

Shape::Shape(const Shape&) {}

Shape::~Shape()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
}

int Shape::getIndexCount()
{
	return m_indexCount;
}

void Shape::Render(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTypeDbgShape);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}