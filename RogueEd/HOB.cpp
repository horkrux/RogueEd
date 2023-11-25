#include "HOB.h"

HOB::HOB() 
{
	int m_numObjects = 0;
	int m_offsetFaces = 0;
}

HOB::HOB(const HOB& other) {}

HOB::~HOB() 
{
	for (int i = 0; i < m_numObjects; i++)
	{
		if (m_objects[i].vertexBuffer)
		{
			m_objects[i].vertexBuffer->Release();
			m_objects[i].vertexBuffer = 0;
		}

		if (m_objects[i].indexBuffer)
		{
			m_objects[i].indexBuffer->Release();
			m_objects[i].indexBuffer = 0;
		}

		//vector<Vertex>().swap(m_objects[i].allVertices);
		//vector<uint16_t>().swap(m_objects[i].allIndices);

		for (int j = 0; j < m_objects[i].numFaceGroups; j++)
		{
			//vector<uint16_t>().swap(m_objects[i].faceGroups[j].indices);

			/*if (m_objects[i].faceGroups[j].numVertices)
				delete[] m_objects[i].faceGroups[j].vertices;*/

			if (m_objects[i].faceGroups[j].vertexBuffer)
			{
				//m_objects[i].faceGroups[j].vertexBuffer->Release();
				//m_objects[i].faceGroups[j].vertexBuffer = 0;
			}

			if (m_objects[i].faceGroups[j].indexBuffer)
			{
				//m_objects[i].faceGroups[j].indexBuffer->Release();
				//m_objects[i].faceGroups[j].indexBuffer = 0;
			}
		}

		if (m_objects[i].faceGroups)
			delete[] m_objects[i].faceGroups;

		/*if (m_objects[i].numIndicesPerFaceGroup)
			delete[] m_objects[i].numIndicesPerFaceGroup;

		if (m_objects[i].numVerticesPerFaceGroup)
			delete[] m_objects[i].numVerticesPerFaceGroup;

		if (m_objects[i].drawInfo.indexCounts)
			delete[] m_objects[i].drawInfo.indexCounts;

		if (m_objects[i].drawInfo.vertexCounts)
			delete[] m_objects[i].drawInfo.vertexCounts;*/

		if (m_objects[i].drawInfo.SHITFUCKTWO)
			delete[] m_objects[i].drawInfo.SHITFUCKTWO;

		for (int j = 0; j < m_objects[i].drawInfo.numMaterials; j++)
		{
			vector<Vertex>().swap(m_objects[i].allVerticesByMat[j]);
		}
		vector<vector<Vertex>>().swap(m_objects[i].allVerticesByMat);
		
	}

	if (m_materialBundle)
		delete m_materialBundle;

	if (m_objects)
		delete[] m_objects;

	vector<char>().swap(m_fileContents);
	
}

int HOB::initialize(ID3D11Device1* device, vector<char> fileContents)
{
	m_fileContents = fileContents;
	char* contents = m_fileContents.data();
	m_numObjects = *(int*)contents;

	m_objects = new ObjectNew[m_numObjects];

	for (int i = 0; i < m_numObjects; i++)
	{
		char* pObject = contents + 8 + (0x74 * i);
		m_objects[i].name = (char*)pObject;

		char* pFacegroupHeader = contents + *(pObject + 0x14);
		m_objects[i].numFaceGroups = *(short*)pFacegroupHeader;
		
		m_objects[i].numIndicesPerFaceGroup = new int[m_objects[i].numFaceGroups];
		m_objects[i].numVerticesPerFaceGroup = new int[m_objects[i].numFaceGroups];
		m_objects[i].faceGroups = new FaceGroup[m_objects[i].numFaceGroups];

		m_objects[i].drawInfo.faceGroupCount = m_objects[i].numFaceGroups;
		m_objects[i].drawInfo.indexCounts = new int[m_objects[i].numFaceGroups];
		m_objects[i].drawInfo.vertexCounts = new int[m_objects[i].numFaceGroups];

		char* pFaceGroup;
		char* pFaceGroup2;
		char* pFaceBlock;
		char* pVertexBlock;

		int vertexCounter = 0;

		for (int j = 0; j < m_objects[i].numFaceGroups; j++)
		{
			pFaceGroup = contents + *(int*)(pFacegroupHeader + 8 + 8 * j);

			//check if there even is a valid pointer
			if (*(int*)(pFaceGroup + 0x10))
			{
				pFaceGroup2 = contents + *(int*)(pFaceGroup + 0x10);
			}
			else {
				m_objects[i].drawInfo.indexCounts[j] = 0;
				m_objects[i].drawInfo.vertexCounts[j] = 0;
				continue;
			}

			m_objects[i].faceGroups[j].numVertices = *(int*)(pFaceGroup2 + 0x14);
			pFaceBlock = contents + *(int*)(pFaceGroup2 + 0x20);
			pVertexBlock = contents + *(int*)(pFaceGroup2 + 0x24);

			m_objects[i].faceGroups[j].vertices = new Vertex[m_objects[i].faceGroups[j].numVertices];
			m_objects[i].faceGroups[j].numFaces = *(int*)(pFaceBlock + 0xC);

			int indexOffset = 0;
			int extraFaces = 0;
			int faceCounter = m_objects[i].faceGroups[j].numFaces;

			char* pFace = contents + *(int*)(pFaceBlock + 8);

			while (faceCounter)
			{
				
				int flags = *(int*)pFace;
				
				int offset = (*(pFace + 7)) * 4;

				uint16_t index1 = *(uint16_t*)(pFace + 0xC);
				uint16_t index2 = *(uint16_t*)(pFace + 0xC + 2);
				uint16_t index3 = *(uint16_t*)(pFace + 0xC + 4);
				uint16_t index4 = *(uint16_t*)(pFace + 0xC + 6);

				int offsetExtraBytes = 0;

				if (flags & 0x40)
					offsetExtraBytes = 8;

				uint16_t matIdx = *(uint16_t*)(pFace + 0xA);

				//split up the STUPID FUCKING QUADS
				if (flags & 8)
				{
					m_objects[i].faceGroups[j].indices.push_back(index1);
					m_objects[i].faceGroups[j].indices.push_back(index2);
					m_objects[i].faceGroups[j].indices.push_back(index3);
					m_objects[i].faceGroups[j].indices.push_back(index1);
					m_objects[i].faceGroups[j].indices.push_back(index3);
					m_objects[i].faceGroups[j].indices.push_back(index4);

					//test
					m_objects[i].allIndices.push_back(index1);
					m_objects[i].allIndices.push_back(index2);
					m_objects[i].allIndices.push_back(index3);
					m_objects[i].allIndices.push_back(index1);
					m_objects[i].allIndices.push_back(index3);
					m_objects[i].allIndices.push_back(index4);

					//set vertex colors and texture uvw
					if (flags & 0x20)
					{
						if (flags & 0x10)
						{
							m_objects[i].vertexColors[index1 + vertexCounter] = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index2 + vertexCounter] = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x18) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index3 + vertexCounter] = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index4 + vertexCounter] = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x20) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 2) / 255.0f, 0.0f);

							if (flags & 4)
							{
								if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index2 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index2 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x28) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2A) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index3 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index3 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x2C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2E) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index4 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index4 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x30) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x32) / 4096.0f, matIdx);
							}
						}
						else {
							m_objects[i].vertexColors[index1 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 1.0f);
							m_objects[i].vertexColors[index2 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 1.0f);
							m_objects[i].vertexColors[index3 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 1.0f);
							m_objects[i].vertexColors[index4 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 1.0f);
						
							if (flags & 4)
							{
								if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index2 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index2 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index3 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index3 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index4 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index4 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f, matIdx);
							}
						}
					}
					else {
						m_objects[i].vertexColors[index1 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
						m_objects[i].vertexColors[index2 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
						m_objects[i].vertexColors[index3 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
						m_objects[i].vertexColors[index4 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

						if (flags & 4)
						{
							if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
							m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x14) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x16) / 4096.0f, matIdx);
							if (m_objects[i].texUVW[index2 + vertexCounter].z == -431602080.)
							m_objects[i].texUVW[index2 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f, matIdx);
							if (m_objects[i].texUVW[index3 + vertexCounter].z == -431602080.)
							m_objects[i].texUVW[index3 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f, matIdx);
							if (m_objects[i].texUVW[index4 + vertexCounter].z == -431602080.)
							m_objects[i].texUVW[index4 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f, matIdx);
						}
					}

					extraFaces++;
				}
				else {
					m_objects[i].faceGroups[j].indices.push_back(index1);
					m_objects[i].faceGroups[j].indices.push_back(index2);
					m_objects[i].faceGroups[j].indices.push_back(index3);

					//test
					m_objects[i].allIndices.push_back(index1);
					m_objects[i].allIndices.push_back(index2);
					m_objects[i].allIndices.push_back(index3);

					if (flags & 0x20)
					{
						if (flags & 0x10)
						{
							m_objects[i].vertexColors[index1 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index2 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x18) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x18 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x18 + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index3 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, 0.0f);
						
							if (flags & 4)
							{
								if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index2 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index2 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index3 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index3 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x28) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2A) / 4096.0f, matIdx);
							}
						}
						else {
							m_objects[i].vertexColors[index1 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index2 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 0.0f);
							m_objects[i].vertexColors[index3 + vertexCounter] = XMVectorSet(*(unsigned char*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(unsigned char*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, 0.0f);
						
							if (flags & 4)
							{
								if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f, matIdx);
								if (m_objects[i].texUVW[index1 + vertexCounter].z == -431602080.)
								m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f, matIdx);
							}
						}
					}
					else {
						m_objects[i].vertexColors[index1 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
						m_objects[i].vertexColors[index2 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
						m_objects[i].vertexColors[index3 + vertexCounter] = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

						if (flags & 4)
						{
							m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x14) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x16) / 4096.0f, matIdx);
							m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f, matIdx);
							m_objects[i].texUVW[index1 + vertexCounter] = XMFLOAT3(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f, matIdx);
						}
					}
				}

				pFace += offset;
				faceCounter--;
			}

			for (int k = 0; k < m_objects[i].faceGroups[j].numVertices; k++)
			{
				m_objects[i].faceGroups[j].vertices[k].x = (*(short*)(pVertexBlock + k * 8)) / 4000.0f;
				m_objects[i].faceGroups[j].vertices[k].y = (*(short*)(pVertexBlock + k * 8 + 2)) / -4000.0f;
				m_objects[i].faceGroups[j].vertices[k].z = (*(short*)(pVertexBlock + k * 8 + 4)) / 4000.0f;
				m_objects[i].faceGroups[j].vertices[k].w = *(short*)(pVertexBlock + k * 8 + 6);

				//test
				Vertex* vertex = new Vertex;
				vertex->x = m_objects[i].faceGroups[j].vertices[k].x;
				vertex->y = m_objects[i].faceGroups[j].vertices[k].y;
				vertex->z = m_objects[i].faceGroups[j].vertices[k].z;
				vertex->w = m_objects[i].faceGroups[j].vertices[k].w;
				m_objects[i].allVertices.push_back(*vertex);
			}

			//m_objects[i].numTotalIndices += m_objects[i].faceGroups[j].indices.size();
			//m_objects[i].numTotalVertices += m_objects[i].faceGroups[j].numVertices;
			//m_objects[i].numIndicesPerFaceGroup[j] = m_objects[i].faceGroups[j].indices.size();
			//m_objects[i].numVerticesPerFaceGroup[j] = m_objects[i].faceGroups[j].numVertices;
			m_objects[i].drawInfo.indexCounts[j] = m_objects[i].faceGroups[j].indices.size();
			m_objects[i].drawInfo.vertexCounts[j] = m_objects[i].faceGroups[j].numVertices;
			m_objects[i].faceGroups[j].numFaces += extraFaces;

			vertexCounter += m_objects[i].faceGroups[j].numVertices;
		}
	}

	if (initializeBuffers(device))
	{
		return 1;
	}

	return 0;
}

int HOB::initializeSHIT(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, vector<char> hobContents, vector<char> hmtContents)
{
	m_materialBundle = new HMT();
	if (m_materialBundle->initialize(device, deviceContext, hmtContents))
	{
		return 1;
	}

	if (!m_materialBundle->getNumMaterials())
	{
		OutputDebugStringA("HMT doesn't contain materials???!!!\n");
		return 1;
	}

	int num_materials = m_materialBundle->getNumMaterials();

	m_fileContents = hobContents;
	char* contents = m_fileContents.data();
	m_numObjects = *(int*)contents;

	m_objects = new ObjectNew[m_numObjects];

	for (int i = 0; i < m_numObjects; i++)
	{
		char* pObject = contents + 8 + (0x74 * i);
		m_objects[i].name = (char*)pObject;

		char* pFacegroupHeader = contents + *(uint16_t*)(pObject + 0x14);
		m_objects[i].numFaceGroups = *(short*)pFacegroupHeader;

		//m_objects[i].numIndicesPerFaceGroup = new int[m_objects[i].numFaceGroups];
		//m_objects[i].numVerticesPerFaceGroup = new int[m_objects[i].numFaceGroups];
		m_objects[i].faceGroups = new FaceGroup[m_objects[i].numFaceGroups];

		m_objects[i].drawInfo.faceGroupCount = m_objects[i].numFaceGroups;
		//m_objects[i].drawInfo.indexCounts = new int[m_objects[i].numFaceGroups];
		//m_objects[i].drawInfo.vertexCounts = new int[m_objects[i].numFaceGroups];
		//m_objects[i].drawInfo.SHITFUCK = new int* [m_objects[i].numFaceGroups];
		m_objects[i].drawInfo.SHITFUCKTWO = new int[num_materials];
		m_objects[i].drawInfo.numMaterials = num_materials;

		m_objects[i].allVerticesByMat.resize(num_materials);
		//m_objects[i].allVerticesByMat = vector<vector<Vertex>>(num_materials);
		for (int j = 0; j < num_materials; j++)
		{
			m_objects[i].allVerticesByMat[j].reserve(1000); //I hope this will be enough for all objects
			//m_objects[i].allVerticesByMat.push_back({});
			m_objects[i].drawInfo.SHITFUCKTWO[j] = 0;
		}
		 

		char* pFaceGroup;
		char* pFaceGroup2;
		char* pFaceBlock;
		char* pVertexBlock;

		int vertexCounter = 0;

		for (int j = 0; j < m_objects[i].numFaceGroups; j++)
		{
			/*m_objects[i].drawInfo.SHITFUCK[j] = new int[num_materials];
			for (int k = 0; k < num_materials; k++)
			{
				m_objects[i].drawInfo.SHITFUCK[j][k] = 0;
			}*/

			pFaceGroup = contents + *(int*)(pFacegroupHeader + 8 + 8 * j);

			m_objects[i].faceGroups[j].transform = *(XMFLOAT3*)(pFaceGroup + 0x78);
			//check if there even is a valid pointer
			if (*(int*)(pFaceGroup + 0x10))
			{
				pFaceGroup2 = contents + *(int*)(pFaceGroup + 0x10);
			}
			else {
				//m_objects[i].drawInfo.indexCounts[j] = 0;
				//m_objects[i].drawInfo.vertexCounts[j] = 0;
				//m_objects[i].faceGroups[j].numVertices = 0;
				continue;
			}

			m_objects[i].faceGroups[j].numVertices = *(int*)(pFaceGroup2 + 0x14);
			pFaceBlock = contents + *(int*)(pFaceGroup2 + 0x20);
			pVertexBlock = contents + *(int*)(pFaceGroup2 + 0x24);

			//m_objects[i].faceGroups[j].vertices = new Vertex[m_objects[i].faceGroups[j].numVertices];
			m_objects[i].faceGroups[j].numFaces = *(int*)(pFaceBlock + 0xC);

			int indexOffset = 0;
			int extraFaces = 0;
			int faceCounter = m_objects[i].faceGroups[j].numFaces;

			char* pFace = contents + *(int*)(pFaceBlock + 8);

			uint16_t currIndex = 0;
			uint16_t matIdx = 0;

			while (faceCounter)
			{
				int flags = *(int*)pFace;

				int offset = (*(BYTE*)(pFace + 7)) * 4;

				uint16_t index1 = *(uint16_t*)(pFace + 0xC);
				uint16_t index2 = *(uint16_t*)(pFace + 0xC + 2);
				uint16_t index3 = *(uint16_t*)(pFace + 0xC + 4);
				uint16_t index4 = *(uint16_t*)(pFace + 0xC + 6);

				int offsetExtraBytes = 0;

				if (flags & 0x40 && flags & 8)
				{
					offsetExtraBytes = 8;
				}
				else if (flags & 0x40)
				{
					offsetExtraBytes = 8; //undecided on this
				}
					

				matIdx = *(uint16_t*)(pFace + 0xA);

				//split up the STUPID FUCKING QUADS
				if (flags & 8)
				{
					/*m_objects[i].faceGroups[j].indices.push_back(index1);
					m_objects[i].faceGroups[j].indices.push_back(index2);
					m_objects[i].faceGroups[j].indices.push_back(index3);
					m_objects[i].faceGroups[j].indices.push_back(index1);
					m_objects[i].faceGroups[j].indices.push_back(index3);
					m_objects[i].faceGroups[j].indices.push_back(index4);*/

					//test
					/*m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);*/

					//m_objects[i].drawInfo.SHITFUCK[j][matIdx] += 6;
					m_objects[i].drawInfo.SHITFUCKTWO[matIdx] += 6;

					Vertex vertex1;
					Vertex vertex2;
					Vertex vertex3;
					Vertex vertex4;
					Vertex vertex5;
					Vertex vertex6;

					vertex1.texUV = { 0.0f, 0.0f };
					vertex2.texUV = { 0.0f, 0.0f };
					vertex3.texUV = { 0.0f, 0.0f };
					vertex4.texUV = { 0.0f, 0.0f };
					vertex5.texUV = { 0.0f, 0.0f };
					vertex6.texUV = { 0.0f, 0.0f };
					
					//set vertex colors and texture uv
					if (flags & 0x20)
					{
						if (flags & 0x10)
						{
							vertex1.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex2.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x18) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 3) / 255.0f);
							vertex3.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 3) / 255.0f);
							vertex4.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex5.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 3) / 255.0f);
							vertex6.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x20) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 3) / 255.0f);

							if (flags & 4)
							{
									vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
									vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x28) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2A) / 4096.0f);
									vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x2C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2E) / 4096.0f);
									vertex4.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
									vertex5.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x2C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2E) / 4096.0f);
									vertex6.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x30) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x32) / 4096.0f);
							}
						}
						else {
							vertex1.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex2.color = vertex1.color;
							vertex3.color = vertex1.color;
							vertex4.color = vertex1.color;
							vertex5.color = vertex1.color;
							vertex6.color = vertex1.color;

							if (flags & 4)
							{
								vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f);
								vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f);
								vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f);
								vertex4.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f);
								vertex5.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f);
								vertex6.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
							}
						}
					}
					else {
						if (flags & 0x10)
						{
							vertex1.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex2.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x18) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 3) / 255.0f);
							vertex3.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 3) / 255.0f);
							vertex4.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex5.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 3) / 255.0f);
							vertex6.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x20) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x20 + 3) / 255.0f);

							if (flags & 4)
							{
								vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
								vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x28) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2A) / 4096.0f);
								vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x2C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2E) / 4096.0f);
								vertex4.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
								vertex5.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x2C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2E) / 4096.0f);
								vertex6.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x30) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x32) / 4096.0f);
							}
						}
						else {
							if (offsetExtraBytes)
							{
								vertex1.color = XMVectorSet(*(BYTE*)(pFace + 0x18) / 255.0f, *(BYTE*)(pFace + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + 0x18 + 2) / 255.0f, *(BYTE*)(pFace + 0x18 + 3) / 255.0f);
								vertex2.color = vertex1.color;
								vertex3.color = vertex1.color;
								vertex4.color = vertex1.color;
								vertex5.color = vertex1.color;
								vertex6.color = vertex1.color;
							}
							else {
								vertex1.color = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
								vertex2.color = vertex1.color;
								vertex3.color = vertex1.color;
								vertex4.color = vertex1.color;
								vertex5.color = vertex1.color;
								vertex6.color = vertex1.color;
							}

							if (flags & 4)
							{
								vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x14) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x16) / 4096.0f);
								vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f);
								vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f);
								vertex4.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x14) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x16) / 4096.0f);
								vertex5.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f);
								vertex6.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f);
							}
						}
					}
					
					vertex1.x = (*(short*)(pVertexBlock + index1 * 8)) / 256.f;//4000.0f;
					vertex1.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex1.y = (*(short*)(pVertexBlock + index1 * 8 + 2)) / 256.f;//4000.0f;
					vertex1.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex1.y *= -1;
					vertex1.z = (*(short*)(pVertexBlock + index1 * 8 + 4)) / 256.f;//4000.0f;
					vertex1.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex1.w = *(short*)(pVertexBlock + index1 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex1);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex1);
					vertex2.x = (*(short*)(pVertexBlock + index2 * 8)) / 256.f;//4000.0f;
					vertex2.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex2.y = (*(short*)(pVertexBlock + index2 * 8 + 2)) / 256.f;//4000.0f;
					vertex2.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex2.y *= -1;
					vertex2.z = (*(short*)(pVertexBlock + index2 * 8 + 4)) / 256.f;//4000.0f;
					vertex2.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex2.w = *(short*)(pVertexBlock + index2 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex2);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex2);
					vertex3.x = (*(short*)(pVertexBlock + index3 * 8)) / 256.f;//4000.0f;
					vertex3.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex3.y = (*(short*)(pVertexBlock + index3 * 8 + 2)) / 256.f;//4000.0f;
					vertex3.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex3.y *= -1;
					vertex3.z = (*(short*)(pVertexBlock + index3 * 8 + 4)) / 256.f;//4000.0f;
					vertex3.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex3.w = *(short*)(pVertexBlock + index3 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex3);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex3);
					vertex4.x = (*(short*)(pVertexBlock + index1 * 8)) / 256.f;//4000.0f;
					vertex4.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex4.y = (*(short*)(pVertexBlock + index1 * 8 + 2)) / 256.f;//4000.0f;
					vertex4.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex4.y *= -1;
					vertex4.z = (*(short*)(pVertexBlock + index1 * 8 + 4)) / 256.f;//4000.0f;
					vertex4.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex4.w = *(short*)(pVertexBlock + index1 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex4);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex4);
					vertex5.x = (*(short*)(pVertexBlock + index3 * 8)) / 256.f;//4000.0f;
					vertex5.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex5.y = (*(short*)(pVertexBlock + index3 * 8 + 2)) / 256.f;//4000.0f;
					vertex5.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex5.y *= -1;
					vertex5.z = (*(short*)(pVertexBlock + index3 * 8 + 4)) / 256.f;//4000.0f;
					vertex5.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex5.w = *(short*)(pVertexBlock + index3 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex5);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex5);
					vertex6.x = (*(short*)(pVertexBlock + index4 * 8)) / 256.f;//4000.0f;
					vertex6.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex6.y = (*(short*)(pVertexBlock + index4 * 8 + 2)) / 256.f;//4000.0f;
					vertex6.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex6.y *= -1;
					vertex6.z = (*(short*)(pVertexBlock + index4 * 8 + 4)) / 256.f;//4000.0f;
					vertex6.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex6.w = *(short*)(pVertexBlock + index4 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex6);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex6);

					extraFaces++;
				}
				else {
					/*m_objects[i].faceGroups[j].indices.push_back(index1);
					m_objects[i].faceGroups[j].indices.push_back(index2);
					m_objects[i].faceGroups[j].indices.push_back(index3);*/

					//test
					/*m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);
					m_objects[i].allIndices.push_back(currIndex++);*/

					//m_objects[i].drawInfo.SHITFUCK[j][matIdx] += 3;
					m_objects[i].drawInfo.SHITFUCKTWO[matIdx] += 3;

					Vertex vertex1;
					Vertex vertex2;
					Vertex vertex3;

					vertex1.texUV = { 0.0f, 0.0f };
					vertex2.texUV = { 0.0f, 0.0f };
					vertex3.texUV = { 0.0f, 0.0f };
					
					if (flags & 0x20)
					{
						if (flags & 0x10)
						{
							vertex1.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex2.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x18) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 3) / 255.0f);
							vertex3.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 3) / 255.0f);

							if (flags & 4)
							{
									vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f);
									vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
									vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x28) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2A) / 4096.0f);
							}
						}
						else {
							vertex1.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex2.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex3.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);

							if (flags & 4)
							{
									vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f);
									vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f);
									vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f);
							}
						}
					}
					else {
						if (flags & 0x10)
						{
							vertex1.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x14) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x14 + 3) / 255.0f);
							vertex2.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x18) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x18 + 3) / 255.0f);
							vertex3.color = XMVectorSet(*(BYTE*)(pFace + offsetExtraBytes + 0x1C) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 1) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 2) / 255.0f, *(BYTE*)(pFace + offsetExtraBytes + 0x1C + 3) / 255.0f);

							if (flags & 4)
							{
								vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x20) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x22) / 4096.0f);
								vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x24) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x26) / 4096.0f);
								vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x28) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x2A) / 4096.0f);
							}
						}
						else {
							if (offsetExtraBytes)
							{
								vertex1.color = XMVectorSet(*(BYTE*)(pFace + 0x18) / 255.0f, *(BYTE*)(pFace + 0x18 + 1) / 255.0f, *(BYTE*)(pFace + 0x18 + 2) / 255.0f, *(BYTE*)(pFace + 0x18 + 3) / 255.0f);
								vertex2.color = vertex1.color;
								vertex3.color = vertex1.color;
							}
							else {
								vertex1.color = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
								vertex2.color = vertex1.color;
								vertex3.color = vertex1.color;
							}

							if (flags & 4)
							{
								vertex1.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x14) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x16) / 4096.0f);
								vertex2.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x18) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1A) / 4096.0f);
								vertex3.texUV = XMFLOAT2(*(short*)(pFace + offsetExtraBytes + 0x1C) / 4096.0f, *(short*)(pFace + offsetExtraBytes + 0x1E) / 4096.0f);
							}
						}
					}

					vertex1.x = (*(short*)(pVertexBlock + index1 * 8)) / 256.f;//4000.0f;
					vertex1.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex1.y = (*(short*)(pVertexBlock + index1 * 8 + 2)) / 256.f;//4000.0f;
					vertex1.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex1.y *= -1;
					vertex1.z = (*(short*)(pVertexBlock + index1 * 8 + 4)) / 256.f;//4000.0f;
					vertex1.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex1.w = *(short*)(pVertexBlock + index1 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex1);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex1);
					vertex2.x = (*(short*)(pVertexBlock + index2 * 8)) / 256.f;//4000.0f;
					vertex2.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex2.y = (*(short*)(pVertexBlock + index2 * 8 + 2)) / 256.f;//4000.0f;
					vertex2.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex2.y *= -1;
					vertex2.z = (*(short*)(pVertexBlock + index2 * 8 + 4)) / 256.f;//4000.0f;
					vertex2.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex2.w = *(short*)(pVertexBlock + index2 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex2);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex2);
					vertex3.x = (*(short*)(pVertexBlock + index3 * 8)) / 256.f;//4000.0f;
					vertex3.x += m_objects[i].faceGroups[j].transform.x / 16;
					vertex3.y = (*(short*)(pVertexBlock + index3 * 8 + 2)) / 256.f;//4000.0f;
					vertex3.y += m_objects[i].faceGroups[j].transform.y / 16;
					vertex3.y *= -1;
					vertex3.z = (*(short*)(pVertexBlock + index3 * 8 + 4)) / 256.f;//4000.0f;
					vertex3.z += m_objects[i].faceGroups[j].transform.z / 16;
					vertex3.w = *(short*)(pVertexBlock + index3 * 8 + 6);
					//m_objects[i].allVertices.push_back(vertex3);
					m_objects[i].allVerticesByMat[matIdx].push_back(vertex3);
				}

				pFace += offset;
				faceCounter--;
			}

			//for (int k = 0; k < m_objects[i].faceGroups[j].numVertices; k++)
			//{
			//	m_objects[i].faceGroups[j].vertices[k].x = (*(short*)(pVertexBlock + k * 8)) / 4000.0f;
			//	m_objects[i].faceGroups[j].vertices[k].y = (*(short*)(pVertexBlock + k * 8 + 2)) / -4000.0f;
			//	m_objects[i].faceGroups[j].vertices[k].z = (*(short*)(pVertexBlock + k * 8 + 4)) / 4000.0f;
			//	m_objects[i].faceGroups[j].vertices[k].w = *(short*)(pVertexBlock + k * 8 + 6);

			//	//test
			//	Vertex* vertex = new Vertex;
			//	vertex->x = m_objects[i].faceGroups[j].vertices[k].x;
			//	vertex->y = m_objects[i].faceGroups[j].vertices[k].y;
			//	vertex->z = m_objects[i].faceGroups[j].vertices[k].z;
			//	vertex->w = m_objects[i].faceGroups[j].vertices[k].w;
			//	//m_objects[i].allVertices.push_back(*vertex);
			//}

			//m_objects[i].numTotalIndices += m_objects[i].faceGroups[j].indices.size();
			//m_objects[i].numTotalVertices += m_objects[i].faceGroups[j].numVertices;
			//m_objects[i].numIndicesPerFaceGroup[j] = m_objects[i].faceGroups[j].indices.size();
			//m_objects[i].numVerticesPerFaceGroup[j] = m_objects[i].faceGroups[j].numVertices;
			//m_objects[i].drawInfo.indexCounts[j] = currIndex;//m_objects[i].faceGroups[j].indices.size();
			//m_objects[i].drawInfo.vertexCounts[j] = currIndex;//m_objects[i].faceGroups[j].numVertices;
			//m_objects[i].faceGroups[j].numFaces += extraFaces;

			//vertexCounter += m_objects[i].faceGroups[j].numVertices;
		}
	}

	if (initializeBuffers(device))
	{
		return 1;
	}

	return 0;
}

int HOB::getIndexCount()
{
	return 0;
}
int HOB::initializeBuffers(ID3D11Device1* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	VertexTypeObject* vertices;
	uint16_t* indices;

	HRESULT result;

	//combine vertices and indices of all face groups

	for (int i = 0; i < m_numObjects; i++)
	{
		int numVertices = 0;// = m_objects[0].allVertices.size();
		int numIndices = 0;// = m_objects[0].allIndices.size();

		for (int j = 0; j < m_objects[i].drawInfo.numMaterials; j++)
		{
			numVertices += m_objects[i].drawInfo.SHITFUCKTWO[j];
		}

		numIndices = numVertices;

		vertices = new VertexTypeObject[numVertices];
		if (!vertices)
		{
			return 1;
		}

		indices = new uint16_t[numIndices];
		if (!indices)
		{
			return 1;
		}

		uint16_t vertexCount = 0;
		int indexCount = 0;

		for (int j = 0; j < m_objects[i].drawInfo.numMaterials; j++)
		{
			for (int k = 0; k < m_objects[i].allVerticesByMat[j].size(); k++)
			{
				vertices[vertexCount].position = XMVectorSet(m_objects[i].allVerticesByMat[j][k].x, m_objects[i].allVerticesByMat[j][k].y, m_objects[i].allVerticesByMat[j][k].z, 1.f);
				vertices[vertexCount].color = m_objects[i].allVerticesByMat[j][k].color;
				vertices[vertexCount].texUV = m_objects[i].allVerticesByMat[j][k].texUV;
				indices[vertexCount] = vertexCount;

				if (m_materialBundle->materialHasTex(j)) {
					vertices[vertexCount].shit = 1;
				}
				else {
					vertices[vertexCount].shit = 0;
				}

				vertexCount++;
			}
		}

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexTypeObject) * numVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_objects[i].vertexBuffer);
		if (FAILED(result))
		{
			return 1;
		}

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = 2 * numIndices;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_objects[i].indexBuffer);
		if (FAILED(result))
		{
			return 1;
		}

		delete[] indices;
		indices = 0;
		delete[] vertices;
		vertices = 0;
	}

	

	return 0;
}

void HOB::render(ID3D11DeviceContext1* deviceContext, int objIndex)
{
	renderBuffers(deviceContext, objIndex);
}

void HOB::renderBuffers(ID3D11DeviceContext1* deviceContext, int objIndex)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTypeObject);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_objects[objIndex].vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_objects[objIndex].indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

HOB::ObjectNew* HOB::getObjects()
{
	return m_objects;
}

int HOB::getNumObjects()
{
	return m_numObjects;
}

HMT* HOB::getMaterialBundle()
{
	return m_materialBundle;
}

vector<string> getObjectNames(vector<char>* hobfilecontents)
{
	BYTE* contents = (BYTE*)hobfilecontents->data();

	int numObjects = *(int*)contents;
	vector<string> objectNames;

	for (int i = 0; i < numObjects; i++)
	{
		objectNames.push_back((char*)(contents + 8 + i * 116));
	}

	return objectNames;
}