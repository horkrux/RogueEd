#include "Spline.h"

Spline::Spline(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
    m_points = NULL;

    vertexBuffer = 0;
    indexBuffer = 0;
    //D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    //D3D11_SUBRESOURCE_DATA vertexData, indexData;
    //vector<VertexTypeDbgShape> vertices;
    //vector<uint16_t> indices;

    //HRESULT result;

    ////stuff goes here

    //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    //vertexBufferDesc.ByteWidth = sizeof(VertexTypeDbgShape) * vertices.size();
    //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //vertexBufferDesc.CPUAccessFlags = 0;
    //vertexBufferDesc.MiscFlags = 0;
    //vertexBufferDesc.StructureByteStride = 0;

    //vertexData.pSysMem = vertices.data();
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

    //m_indexCount = indices.size();
}

Spline::Spline(const Spline& cpy) {

}

Spline::~Spline()
{
    if (m_points)
        delete m_points;
}

int Spline::read(char* pThingy)
{
    memcpy(&m_params, pThingy, sizeof(Params));

    if (m_params.offsetName)
    {
        m_name = pThingy + m_params.offsetName;
    }

    m_eventId = m_params.index;
    m_position = m_params.position;

    m_points = new XMVECTOR[m_params.numPoints];

    char* pPoints = pThingy + m_params.offsetPoints;

    for (int i = 0; i < m_params.numPoints; i++)
    {
        m_points[i] = XMVectorSet(m_position.x + *(float*)(pPoints + i * 12), m_position.y + *(float*)(pPoints + i * 12 + 4), m_position.z + *(float*)(pPoints + i * 12 + 8), 1.f);
    }

    //+40h  EndReached_ModCount
    //+48h  EndReached_SetFlag
    //+4Ch  EndReached_SetFlag_to

    return 0;
}

void Spline::CalcSpline(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext)
{
    XMVECTOR p0 = m_points[0];
    XMVECTOR p1 = m_points[1];
    XMVECTOR p2 = m_points[2];
    XMVECTOR p3 = m_points[3];

    float t0 = 0.0f;
    float t1 = GetT(t0, 0.5f, m_points[0], m_points[1]);
    float t2 = GetT(t1, 0.5f, m_points[1], m_points[2]);
    float t3 = GetT(t2, 0.5f, m_points[2], m_points[3]);

    for (int i = 0; i < m_params.numPoints - 3; i++)
    {
        p0 = m_points[i];
        p1 = m_points[i + 1];
        p2 = m_points[i + 2];
        p3 = m_points[i + 3];

        for (float t = 0.f; t < 1.05f; t += 0.05f)
        {
            m_splinePoints.push_back(CatMullRom(p0, p1, p2, p3, t));
        }
    }
    

    /*p0 = m_points[1];
    p1 = m_points[2];
    p2 = m_points[3];
    p3 = m_points[4];

    for (float t = 0.f; t < 1.05f; t += 0.05f)
    {
        m_splinePoints.push_back(CatMullRom(p0, p1, p2, p3, t));
    }*/


    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    vector<VertexTypeDbgShape> vertices;
    vector<uint16_t> indices;

    HRESULT result;

    for (int i = 0; i < m_splinePoints.size(); i++)
    {
        vertices.push_back(VertexTypeDbgShape(m_splinePoints[i].m128_f32[0], -m_splinePoints[i].m128_f32[1], m_splinePoints[i].m128_f32[2]));
        indices.push_back(i);
        indices.push_back(i+1);
    }

    indices.pop_back();

    m_color = XMVectorSet(1.f, 0.f, .5f, 1.f);

    //stuff goes here

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
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

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

    m_indexCount = indices.size();

    //m_worldMatrix = XMMatrixRotationRollPitchYaw(0.f, 0.f, 0.f);
    //m_worldMatrix *= XMMatrixScaling(1.f, 1.f, 1.f);
    //m_worldMatrix *= XMMatrixTranslation(0, 0, 0);
}

float Spline::GetT(float t, float alpha, const XMVECTOR& p0, const XMVECTOR& p1)
{
    XMVECTOR powi = XMVectorSet(0, 1.f, 1.f, 1.f);
    XMVectorSetX(powi, alpha * .5f);
    XMVECTOR d = p1 - p0;
    XMVECTOR a = XMVector4Dot(d, d); // Dot product
    XMVECTOR b = XMVectorPow(a, powi);
    return (b.m128_f32[0] + t);
}

float Spline::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

XMVECTOR Spline::CatMullRom(const XMVECTOR& p0, const XMVECTOR& p1, const XMVECTOR& p2, const XMVECTOR& p3, float t, float alpha)
{
    //XMVECTOR shit = p0 * 0.1f;
    
    float t0 = 0.f;
    float t1 = GetT(t0, alpha, p0, p1);
    float t2 = GetT(t1, alpha, p1, p2);
    float t3 = GetT(t2, alpha, p2, p3);
    t = lerp(t1, t2, t);
    XMVECTOR A1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
    XMVECTOR A2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
    XMVECTOR A3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;
    XMVECTOR B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
    XMVECTOR B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;
    XMVECTOR C = (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;
    return C;
}