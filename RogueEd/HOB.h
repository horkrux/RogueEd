#ifndef _HOB_H_
#define _HOB_H_

#include <d3d11_1.h>
#include <vector>
#include <array>
#include <directxmath.h>

#include "DataIO.h"
#include "HMT.h"
#include "ModelCommonTypes.h"

using namespace std;
using namespace DirectX;

class HOB
{
public:
	HOB();
	HOB(const HOB&);
	~HOB();

	int initialize(ID3D11Device1*, vector<char>);
	int initializeSHIT(ID3D11Device1*, ID3D11DeviceContext1*, vector<char>, vector<char>);
	void render(ID3D11DeviceContext1*, int objIndex = 0);
	int getIndexCount();

private:

	int initializeBuffers(ID3D11Device1*);
	void renderBuffers(ID3D11DeviceContext1*, int);

	typedef struct
	{
		char name[16];
		uint32_t offsetFacegroups;
		uint32_t offsetFacegroupHeader;
		uint32_t offsetFacegroupHeader2;
		char unknown0[12];
		int unknown1[3];
		float unknown2;
		char unknown3[12];
		float unknown4[5];
		uint32_t offsetUnknownFaceheader;
		float unknown5[6];
	} Object;

	typedef struct
	{
		short numFacegroups;
		short unknown0;
		//array of facegroup offsets
		//list of stupid floats, ending on -1
	} FacegroupHeader;

	typedef struct
	{
		uint32_t offsetNext;
		uint32_t offsetPrev;
		uint32_t offsetFirst;
		uint32_t offsetLast;
		uint32_t offsetFacegroup2;
		char unknown0[8];
		float unknown1[12];
		int unknown2; //flags: 0: nothing set, 1: unknown3 set, 2: unknown4 set, 4: unknown5 set, 8: unknown6 set
		float unknown3[3];
		float unknown4[3]; //1.0 if not set
		float unknown5[4];
		float unknown6[3];
	} Facegroup;

	typedef struct
	{
		uint32_t offsetFaceDefEnd;
		int unknown0[5];
		int vertexCount;
		int unknown1;
		int unknown2;
		uint32_t offsetFaceBlock;
		uint32_t offsetVertexBlock;
		int unknown3[13];
	} Facegroup2;

	typedef struct
	{
		char unknown0[8];
		uint32_t offsetFace;
		int faceCount;
	} Faceblock;

	typedef struct
	{
		int flags;
		int unknown0;
		uint16_t unknown1;
		uint16_t materialIdx;
		uint16_t indices[4];
		int unknown2[2];
		union colors {
			uint32_t vertexColors4[4];
			uint32_t vertexColors3[3];
			uint32_t faceColor;
		};
		union texCoords {
			uint32_t texCoords4[4];
			uint32_t texCoords3[3];
		};
	} Face;

	typedef struct
	{
		float x;
		float y;
		float z;
		float w;
		XMVECTOR color;
		XMFLOAT2 texUV;
	} Vertex;

	//typedef struct
	//{
	//	XMFLOAT3 position;
	//	float padding = 0;
	//	XMVECTOR color;
	//	XMFLOAT2 texUV;
	//	uint32_t shit;
	//	float padding1 = 0;
	//} VertexType;
	
	typedef struct
	{
		int numFaces;
		int numVertices;
		XMFLOAT3 transform;
		vector<uint16_t> indices;
		Vertex* vertices;
		array<int, 8> shitFUCK;
		ID3D11Buffer* vertexBuffer, * indexBuffer;
	} FaceGroup;

	struct ObjectNew
	{
		char* name;
		short numFaceGroups;
		FaceGroup* faceGroups;
		vector<uint16_t> allIndices;
		vector<Vertex> allVertices;
		int* numVerticesPerFaceGroup;
		int* numIndicesPerFaceGroup;
		XMVECTOR vertexColors[1024];
		XMFLOAT3 texUVW[1024];
		int numTotalVertices;
		int numTotalIndices;
		DrawInfo drawInfo;
		vector<vector<Vertex>> allVerticesByMat;
		ID3D11Buffer* vertexBuffer, * indexBuffer;
	};

	typedef struct
	{
		uint16_t indices[4];
	} FaceNew;

	int m_numObjects;
	int m_offsetFaces;
	ObjectNew* m_objects;
	vector<char> m_fileContents;

	HMT* m_materialBundle;

public:
	ObjectNew* getObjects();
	int getNumObjects();
	HMT* getMaterialBundle();
	
};

//a helper function only retrieve object names
vector<string> getObjectNames(vector<char>*);

#endif