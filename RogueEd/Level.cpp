#include "Level.h"

Level::Level()
{
	m_numObjects = 0;
	m_numSplines = 0;
	m_numTextures = 0;
	m_tileCount = 0;
	m_tile2Tex = 0;
	m_width = 0;
	m_height = 0;
	m_levelId = '0';
}

Level::Level(const Level& cpy) {}

Level::~Level() 
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	vector<VertexTypeTerrain>().swap(verticesHeightmap);
	vector<uint16_t>().swap(indicesHeightmap);

	unordered_multimap<RogueObjectType, RogueObject*>::iterator it = m_renderList.begin();

	while (it != m_renderList.end())
	{
		delete it->second;
		it = m_renderList.erase(it); //TODO: Put this into Graphics
	}

	for (int i = 0; i < m_dbgRenderList.size(); i++)
	{
		delete m_dbgRenderList[i]; //TODO: Put this into Graphics
	}

	vector<DbgShape*>().swap(m_dbgRenderList);

	for (int i = 0; i < m_numTextures; i++)
	{
		if (m_textures[i])
		{
			m_textures[i]->Release();
			m_textures[i] = 0;
		}

		if (m_textureViews[i])
		{
			m_textureViews[i]->Release();
			m_textureViews[i] = 0;
		}
	}

	if (m_tile2Tex)
	{
		delete[] m_tile2Tex;
		m_tile2Tex = 0;
	}
}

unsigned int Level::getNumObjects()
{
	return m_numObjects;
}

void Level::setNumObjects(unsigned int numObjects)
{
	m_numObjects = numObjects;
}

unsigned int Level::getNumSplines()
{
	return m_numSplines;
}

void Level::setNumSplines(unsigned int numSplines)
{
	m_numSplines = numSplines;
}

unsigned int Level::getTileCount()
{
	return m_tileCount;
}

void Level::setTileCount(unsigned int tileCount)
{
	m_tileCount = tileCount;
}

unsigned int Level::getNumTextures()
{
	return m_numTextures;
}

void Level::setNumTextures(unsigned int numTextures)
{
	m_numTextures = numTextures;
}

ID3D11ShaderResourceView** Level::getShaderResView()
{
	return m_textureViews;
}

void Level::setShaderResViews(ID3D11ShaderResourceView** textureViews)
{
	m_textureViews = textureViews;
}

ID3D11Texture2D** Level::getTextures()
{
	return m_textures;
}
void Level::setTextures(ID3D11Texture2D** textures)
{
	m_textures = textures;
}

uint16_t* Level::getTile2Tex()
{
	return m_tile2Tex;
}

void Level::setTile2Tex(uint16_t* tile2Tex)
{
	m_tile2Tex = tile2Tex;
}

XMVECTOR Level::getTerrainDiffuseColor()
{
	return m_terrainDiffuseColor;
}

void Level::setTerrainDiffuseColor(float r, float g, float b, float a)
{
	m_terrainDiffuseColor = XMVectorSet(r, g, b, a);
}

XMVECTOR Level::getObjectDiffuseColor()
{
	return m_objectDiffuseColor;
}

void Level::setObjectDiffuseColor(float r, float g, float b, float a)
{
	m_objectDiffuseColor = XMVectorSet(r, g, b, a);
}

float Level::getTerrainScaleY()
{
	return m_terrainScaleY;
}

void Level::setTerrainScaleY(float scale)
{
	m_terrainScaleY = scale;
}

uint16_t Level::getWidth()
{
	return m_width;
}

void Level::setWidth(uint16_t width)
{
	m_width = width;
}

uint16_t Level::getHeight()
{
	return m_height;
}

void Level::setHeight(uint16_t height)
{
	m_height = height;
}

char Level::getLevelId()
{
	return m_levelId;
}

void Level::setLevelId(char levelId)
{
	m_levelId = levelId;
}

XMMATRIX Level::getTerrainWorldMatrix()
{
	return m_worldMatrix;
}

void Level::setTerrainWorldMatrix(XMMATRIX worldMatrix)
{
	m_worldMatrix = worldMatrix;
}

unordered_multimap<RogueObjectType, RogueObject*>* Level::getRenderList()
{
	return &m_renderList;
}

vector<DbgShape*> Level::getDbgRenderList()
{
	return m_dbgRenderList;
}

void Level::addRenderObject(RogueObject* obj)
{
	m_renderList.insert({ obj->getType(), obj });
}

void Level::removeRenderObject(RogueObjectType key, unsigned int eventId)
{
	unordered_multimap<RogueObjectType, RogueObject*>::iterator it = m_renderList.find(key);
	
	while (it != m_renderList.end())
	{
		if (it->second->getEventId() == eventId)
		{
			m_renderList.erase(it);
		}

		it++;
	}
}

void Level::addDbgRenderObject(DbgShape* shape)
{
	m_dbgRenderList.push_back(shape);
}

void Level::removeDbgRenderObject(unsigned int eventId)
{
	for (int i = 0; i < m_dbgRenderList.size(); i++)
	{
		if (m_dbgRenderList[i] && m_dbgRenderList[i]->getEventId() == eventId)
		{
			m_dbgRenderList.erase(m_dbgRenderList.begin() + i);
		}
	}
}

void Level::setHeightmap(vector<VertexTypeTerrain> vertices, vector<uint16_t> indices)
{
	verticesHeightmap = vertices;
	indicesHeightmap = indices;
}

int Level::initializeHeightmap(ID3D11Device1* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeTerrain) * verticesHeightmap.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = verticesHeightmap.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return 1;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = 2 * indicesHeightmap.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indicesHeightmap.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return 1;
	}
}

void Level::renderHeightmap(ID3D11DeviceContext1* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTypeTerrain);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}