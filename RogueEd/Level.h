#pragma once

#include "RogueObject.h"
#include "DbgShape.h"

#include <vector>
using namespace std;

class Level
{
public:
	Level();
	Level(const Level&);
	~Level();

	unsigned int getNumObjects();
	void setNumObjects(unsigned int);
	unsigned int getNumSplines();
	void setNumSplines(unsigned int);
	unsigned int getTileCount();
	void setTileCount(unsigned int);
	unsigned int getNumTextures();
	void setNumTextures(unsigned int);
	uint16_t* getTile2Tex();
	void setTile2Tex(uint16_t*);

	XMVECTOR getTerrainDiffuseColor();
	void setTerrainDiffuseColor(float, float, float, float);
	XMVECTOR getObjectDiffuseColor();
	void setObjectDiffuseColor(float, float, float, float);
	float getTerrainScaleY();
	void setTerrainScaleY(float);
	uint16_t getWidth();
	void setWidth(uint16_t);
	uint16_t getHeight();
	void setHeight(uint16_t);
	char getLevelId();
	void setLevelId(char);
	XMMATRIX getTerrainWorldMatrix();
	void setTerrainWorldMatrix(XMMATRIX);

	ID3D11ShaderResourceView** getShaderResView();
	void setShaderResViews(ID3D11ShaderResourceView**);
	ID3D11Texture2D** getTextures();
	void setTextures(ID3D11Texture2D**);

	unordered_multimap<RogueObjectType, RogueObject*>* getRenderList();
	vector<DbgShape*> getDbgRenderList();

	void addRenderObject(RogueObject*);
	void removeRenderObject(RogueObjectType, unsigned int);

	void addDbgRenderObject(DbgShape*);
	void removeDbgRenderObject(unsigned int);

	void setHeightmap(vector<VertexTypeTerrain>, vector<uint16_t>);
	int initializeHeightmap(ID3D11Device1*);
	void renderHeightmap(ID3D11DeviceContext1*);

private:
	unsigned int m_numObjects;
	unsigned int m_numSplines;
	unsigned int m_tileCount;
	float m_terrainScaleY;
	XMVECTOR m_terrainDiffuseColor;
	XMVECTOR m_objectDiffuseColor;
	uint16_t m_width;
	uint16_t m_height;

	XMMATRIX m_worldMatrix;

	char m_levelId; //TODO turn this into number 

	int m_numTextures;
	ID3D11Texture2D** m_textures;
	ID3D11ShaderResourceView** m_textureViews;

	uint16_t* m_tile2Tex;

	vector<RogueObject*> m_objects;
	unordered_multimap<RogueObjectType, RogueObject*> m_renderList;
	vector<DbgShape*> m_dbgRenderList;

	vector<VertexTypeTerrain> verticesHeightmap;
	vector<uint16_t> indicesHeightmap;

	ID3D11Buffer* vertexBuffer, * indexBuffer;
};