#include "DbgShapeSphere.h"

Sphere* DbgShapeSphere::m_referenceSphere = NULL;

DbgShapeSphere::DbgShapeSphere(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
    //vertexBuffer = 0;
    //indexBuffer = 0;
    //D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    //D3D11_SUBRESOURCE_DATA vertexData, indexData;
    //vector<VertexTypeDbgShape> vertices;
    //vector<uint16_t> indices;

    //HRESULT result;

    ////http://www.songho.ca/opengl/gl_sphere.html#example_sphere

    //float radius = 1.f;
    //float x, y, z, xz;        // vertex position
    //int sectorCount = 18;
    //int stackCount = 10;

    //float sectorStep = 2 * M_PI / sectorCount;
    //float stackStep = M_PI / stackCount;
    //float sectorAngle, stackAngle;

    //int currIndex = 0;
    //int stackStartIndex = 0;

    //for (int i = 0; i <= stackCount; ++i)
    //{
    //    stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
    //    xz = radius * cosf(stackAngle);             // r * cos(u)
    //    y = radius * sinf(stackAngle);              // r * sin(u)

    //    if (i == 1)
    //        stackStartIndex = currIndex;
    //    // add (sectorCount+1) vertices per stack
    //    // the first and last vertices have same position and normal, but different tex coords
    //    for (int j = 0; j <= sectorCount; ++j)
    //    {
    //        sectorAngle = j * sectorStep;           // starting from 0 to 2pi

    //        // vertex position (x, y, z)
    //        x = xz * cosf(sectorAngle);             // r * cos(u) * cos(v)
    //        z = xz * sinf(sectorAngle);             // r * cos(u) * sin(v)
    //        vertices.push_back(VertexTypeDbgShape(x, y, z));

    //        if (i > 0 && j < sectorCount)
    //        {
    //            indices.push_back(currIndex);
    //            indices.push_back(currIndex + 1);

    //            indices.push_back(currIndex);
    //            indices.push_back(currIndex - stackStartIndex);
    //                
    //        }

    //        currIndex++;
    //    }
    //}

   // indices.push_back(0);
    //indices.push_back(0x25);

    /*for (int i = 0; i <= sectorCount; i++)
    {
        for (int j = 0; j <= stackCount; j++)
        {
            indices.push_back(i * j * sectorCount);
            if (j > 0 && j < stackCount)
                indices.push_back(i * j * sectorCount);
        }
    }*/

    /*vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexTypeDbgShape) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices.data();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = 2 * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);*/

    if (!m_referenceSphere)
    {
        m_referenceSphere = new Sphere(device, deviceContext);
    }

    m_referenceSphere->addReference();

    //m_indexCount = indices.size();

    m_shapeType = DbgShape::ShapeType::SPHERE;
}

DbgShapeSphere::DbgShapeSphere(const DbgShapeSphere& cpy) {}

DbgShapeSphere::~DbgShapeSphere()
{
    if (!m_referenceSphere->removeReference())
        m_referenceSphere = NULL;
}

//this is a shitty test function
int DbgShapeSphere::whatever(char* pThingy, XMFLOAT3 posSpline)
{
    XMFLOAT3 relativePos = *(XMFLOAT3*)pThingy;

    m_position = XMFLOAT3(posSpline.x + relativePos.x, posSpline.y + relativePos.y, posSpline.z + relativePos.z);
    m_color = XMVectorSet(1.0f, 0.5f, 0.f, 1.f);
    m_radius = 1.0f;
    m_params.radius = 1.0f;

    m_worldMatrix = XMMatrixScaling(m_radius / 2, m_radius / 2, m_radius / 2);
    m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);

    m_name = "convoy1";

    return 0;
}

int DbgShapeSphere::read(char* pThingy)
{
    memcpy(&m_params, pThingy, sizeof(Params));

    if (m_params.offsetName)
    {
        m_name = pThingy + m_params.offsetName;
    }

    m_eventId = m_params.index;
    m_position = m_params.position;
    m_radius = m_params.radius;
    //m_params.radius = 1.0f;

    m_worldMatrix = XMMatrixScaling(m_radius / 2, m_radius / 2, m_radius / 2);
    m_worldMatrix *= XMMatrixTranslation(m_position.x, -m_position.y, m_position.z);

    switch (m_params.type)
    {
    case 0x70022:
        m_color = XMVectorSet(.5f, 0.f, .5f, 1.f);
        break;
    case 0x7001A:
        m_color = XMVectorSet(0, 1.f, 0, 1.f);
        break;
    case 0x30003:
        m_color = XMVectorSet(1.0f, 1.0f, 0.f, 1.f);
        break;
    case 0x7003A:
        m_color = XMVectorSet(0, 0, 1.f, 1.f);
        break;
    default:
        m_color = XMVectorSet(1.f, 0.f, 0, 1.f);
    }
    



    return 0;
}

float DbgShapeSphere::getRadius()
{
    return m_radius;
}

void DbgShapeSphere::setRadius(float radius)
{
    m_radius = radius;
}

Sphere* DbgShapeSphere::getReferenceSphere()
{
    return m_referenceSphere;
}