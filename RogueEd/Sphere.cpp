#include "Sphere.h"

int Sphere::m_referenceCount = 0;

Sphere::Sphere(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    vector<VertexTypeDbgShape> vertices;
    vector<uint16_t> indices;

    HRESULT result;

    //http://www.songho.ca/opengl/gl_sphere.html#example_sphere

    float radius = 1.f;
    float x, y, z, xz;        // vertex position
    int sectorCount = 18;
    int stackCount = 10;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    int currIndex = 0;
    int stackStartIndex = 0;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xz = radius * cosf(stackAngle);             // r * cos(u)
        y = radius * sinf(stackAngle);              // r * sin(u)

        if (i == 1)
            stackStartIndex = currIndex;
        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xz * cosf(sectorAngle);             // r * cos(u) * cos(v)
            z = xz * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(VertexTypeDbgShape(x, y, z));

            if (i > 0 && j < sectorCount)
            {
                indices.push_back(currIndex);
                indices.push_back(currIndex + 1);

                indices.push_back(currIndex);
                indices.push_back(currIndex - stackStartIndex);

            }

            currIndex++;
        }
    }

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexTypeDbgShape) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices.data();
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

    m_indexCount = indices.size();
}

Sphere::Sphere(const Sphere&) {}

void Sphere::addReference()
{
	m_referenceCount++;
}

int Sphere::removeReference()
{
	if (!--m_referenceCount)
	{
		delete this;
	}

	return m_referenceCount;
}