#include "LevelIO.h"

LevelIO::LevelIO(){}
LevelIO::LevelIO(const LevelIO&){}
LevelIO::~LevelIO(){}

LevelIO* LevelIO::single = NULL;

LevelIO* LevelIO::getInstance()
{
	if (single == NULL)
		single = new LevelIO();

	return single;
}

int LevelIO::readDat(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, Level* level, vector<char>* datcontents)
{
	char* contents = datcontents->data();

	uint32_t numObjects = *(uint32_t*)contents;
	uint32_t numSplines = *(uint32_t*)(contents + 0x18);
	uint32_t numThingies = *(uint32_t*)(contents + 0x30);
	uint32_t numSphereThingies = *(uint32_t*)(contents + 0x38);

	level->setNumObjects(numObjects);
	int offsetObjectList = *(int*)(contents + 4);
	int offsetSplinesList = *(int*)(contents + 0x1C);
	int offsetThingyList = *(int*)(contents + 0x34);
	int offsetSphereList = *(int*)(contents + 0x3C);

	int* objectOffsets = new int[numObjects];
	int* splineOffsets = new int[numSplines];
	int* thingyOffsets = new int[numThingies];
	int* sphereOffsets = new int[numSphereThingies];

	//memcpy(objectOffsets, contents + offsetObjectList, numObjects * 4);

	for (int i = 0; i < numObjects; i++)
	{
		objectOffsets[i] = *(int*)(contents + offsetObjectList + i * 4) + offsetObjectList;
	}

	char* pObj = 0;

	for (int i = 0; i < numObjects; i++)
	{
		pObj = contents + objectOffsets[i];

		RogueObjectType objType = *(RogueObjectType*)pObj;

		switch (objType)
		{
		case ATAT_TYPE:
		{
			ATAT* atat = new ATAT(device, deviceContext);
			atat->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(atat));
			break;
		}
		case ATST_TYPE:
		{
			ATST* atst = new ATST(device, deviceContext);
			atst->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(atst));
			break;
		}
		case MEGA_TURRET:
		{
			MegaTurret* mega = new MegaTurret(device, deviceContext);
			mega->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(mega));
			break;
		}
		case GUN_TURRET:
		{
			GunTurret* gunt = new GunTurret(device, deviceContext);
			gunt->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(gunt));
			break;
		}
		case NON_DESTRUCTIBLE:
		{
			NonDestructible* nd = new NonDestructible(device, deviceContext, level->getLevelId());
			nd->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(nd));
			break;
		}
		case GENERIC_DESTROYABLE:
		{
			GenericDestroyable* gd = new GenericDestroyable(device, deviceContext, level->getLevelId());
			gd->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(gd));
			break;
		}
		case REB_TRANS:
		{
			RebTrans* rb_trans = new RebTrans(device, deviceContext);
			rb_trans->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(rb_trans));
			break;
		}
		case REB_COMBAT:
		{
			RebCombat* rb_combat = new RebCombat(device, deviceContext);
			rb_combat->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(rb_combat));
			break;
		}
		case PROBE_DROID:
		{
			ProbeDroid* prb_droid = new ProbeDroid(device, deviceContext);
			prb_droid->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(prb_droid));
			break;
		}
		case CIVILIAN:
		{
			Civilian* civ = new Civilian(device, deviceContext);
			civ->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(civ));
			break;
		}
		case STORMTROOPER:
		{
			Stormtrooper* trp = new Stormtrooper(device, deviceContext);
			trp->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(trp));
			break;
		}
		case TIE_BOMBER:
		{
			TIE_Bomber* tiebomber = new TIE_Bomber(device, deviceContext);
			tiebomber->read(pObj);
			level->addRenderObject(reinterpret_cast<RogueObject*>(tiebomber));
			break;
		}

		default:
		{
			char dbgString[128];
			sprintf_s(dbgString, 128, "Unhandled object type: %d\n", objType);
			OutputDebugStringA(dbgString);
		}
		}
	}

	for (int i = 0; i < numSplines; i++)
	{
		splineOffsets[i] = *(int*)(contents + offsetSplinesList + i * 4) + offsetSplinesList;
	}

	char* pSpline;

	//this is very precious rn, change when this shit is reversed
	for (int i = 0; i < numSplines; i++)
	{
		pSpline = contents + splineOffsets[i];

		Spline* shape = new Spline(device, deviceContext);
		shape->read(pSpline);
		//if (shape->getName() == "convoy1")
		{
			shape->CalcSpline(device, deviceContext);
			level->addDbgRenderObject(reinterpret_cast<DbgShape*>(shape));
		}
		

		int offsetPointThingies = *(int*)(pSpline + 0x20);
		int numPointThingies = *(int*)(pSpline + 0x1C);

		char* pPointThingy = pSpline + offsetPointThingies;

		//if (shape->getName() == "convoy1")
		{
			for (int j = 0; j < numPointThingies; j++)
			{
				DbgShapeSphere* sphery = new DbgShapeSphere(device, deviceContext);
				//sphery->setRadius(1.0f);
				sphery->whatever(pPointThingy, shape->getPosition());
				level->addDbgRenderObject(reinterpret_cast<DbgShape*>(sphery));
		
				pPointThingy += 12;
			}
		}
		
	}

	for (int i = 0; i < numThingies; i++)
	{
		thingyOffsets[i] = *(int*)(contents + offsetThingyList + i * 4) + offsetThingyList;
	}

	char* pThingy;

	for (int i = 0; i < numThingies; i++)
	{
		pThingy = contents + thingyOffsets[i];

		if (*(int*)pThingy == 0x60016)
		{
			DbgShapeBox* shape = new DbgShapeBox(device, deviceContext);
			shape->read(pThingy);
			level->addDbgRenderObject(shape);

		}
		else if (*(int*)pThingy == 0x60004)
		{
			DbgShapeBox* shape = new DbgShapeBox(device, deviceContext);
			shape->read(pThingy);
			level->addDbgRenderObject(shape);
		}
		else if (*(int*)pThingy == 0x60005)
		{
			DbgShapeBox* shape = new DbgShapeBox(device, deviceContext);
			shape->read(pThingy);
			level->addDbgRenderObject(shape);
		}
		else if (*(int*)pThingy == 0x60010)
		{
			DbgShapeBox* shape = new DbgShapeBox(device, deviceContext);
			shape->read(pThingy);
			level->addDbgRenderObject(shape);
		}
		else if (*(int*)pThingy == 0x60039)
		{
			DbgShapeBox* shape = new DbgShapeBox(device, deviceContext);
			shape->read(pThingy);
			level->addDbgRenderObject(shape);
		}
	}

	for (int i = 0; i < numSphereThingies; i++)
	{
		sphereOffsets[i] = *(int*)(contents + offsetSphereList + i * 4) + offsetSphereList;
	}

	char* pSphere;

	for (int i = 0; i < numSphereThingies; i++)
	{
		pSphere = contents + sphereOffsets[i];

		if (*(int*)pSphere == 0x70011)
		{
		DbgShapeSphere* shape = new DbgShapeSphere(device, deviceContext);
		shape->read(pSphere);
		level->addDbgRenderObject(reinterpret_cast<DbgShape*>(shape));
		}
		else if (*(int*)pSphere == 0x70018)
		{
			DbgShapeSphere* shape = new DbgShapeSphere(device, deviceContext);
			shape->read(pSphere);
			level->addDbgRenderObject(reinterpret_cast<DbgShape*>(shape));
		}
		else if (*(int*)pSphere == 0x7001A)
		{
			DbgShapeSphere* shape = new DbgShapeSphere(device, deviceContext);
			shape->read(pSphere);
			level->addDbgRenderObject(reinterpret_cast<DbgShape*>(shape));
		}
		else if (*(int*)pSphere == 0x70022)
		{
			DbgShapeSphere* shape = new DbgShapeSphere(device, deviceContext);
			shape->read(pSphere);
			level->addDbgRenderObject(reinterpret_cast<DbgShape*>(shape));
		}
		else if (*(int*)pSphere == 0x7003A)
		{
			DbgShapeSphere* shape = new DbgShapeSphere(device, deviceContext);
			shape->read(pSphere);
			level->addDbgRenderObject(reinterpret_cast<DbgShape*>(shape));
		}
	}

	delete[] objectOffsets;
	delete[] thingyOffsets;
	delete[] sphereOffsets;

	return 0;
}

int LevelIO::readHmp(Level* level, vector<char>* hmpcontents)
{
	char* contents = hmpcontents->data();

	float scaleY = *(float*)(contents + 0x10);
	uint16_t width = *(uint16_t*)(contents + 0x24);
	uint16_t height = *(uint16_t*)(contents + 0x26);

	level->setTerrainScaleY(scaleY);
	level->setWidth(width);
	level->setHeight(height);
	level->setTileCount(width * height);

	uint16_t widthPixels = (width - 1) * 4 + 5;
	uint16_t heightPixels = (height - 1) * 4 + 5;

	int offsetTileData = *(int*)(contents + 0x1C);
	int offsetLightData = *(int*)(contents + 0x20);
	char* pTileData = contents + offsetTileData;
	char* pTiles = contents + 0x28;
	char* pTile = 0;

	int row = -1;
	int column = 0;
	int index = 0;
	uint16_t tileDataIdx = 0;
	//bool BULLSHIT = false;

	vector<VertexTypeTerrain> vertices;
	//vector<uint16_t> indices = { 0, 5, 1, 6, 2, 7, 3, 8, 4, 9, 9, 5, 5, 10, 6, 11, 7, 12, 8, 13, 9, 14, 14, 10, 10, 15, 11, 16, 12, 17, 13, 18, 14, 19, 19, 15, 15, 20, 16, 21, 17, 22, 18, 23, 19, 24 };
	vector<uint16_t> indices = { 0, 1, 5, 6, 10, 11, 15, 16, 20, 21, 21, 1, 1, 2, 6, 7, 11, 12, 16, 17, 21, 22, 22, 2, 2, 3, 7, 8, 12, 13, 17, 18, 22, 23, 23, 3, 3, 4, 8, 9, 13, 14, 18, 19, 23, 24};
	VertexTypeTerrain vertex;
	vertex.normal = XMVectorSet(0, 0, 0, 1);

	uint16_t* tile2Tex = new uint16_t[width * height];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			tileDataIdx = *(uint16_t*)(pTiles + (i * width + j) * 2);
			pTile = pTileData + tileDataIdx * 0x1E;

			vertex.position.m128_f32[2] = -1;
			//vertex.position.m128_f32[1] = 0;
			tile2Tex[i * width + j] = *(uint16_t*)pTile;

			for (int k = 0; k < 5; k++)
			{
				//vertex.shit = 1;
				vertex.color = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
				vertex.texUV.y = 1.0f - k * (1 / 4.0f);
				vertex.texUV.x = 0.0f;
				vertex.position.m128_f32[0] = -1.0f;
				vertex.position.m128_f32[1] = *(pTile + 5 + k * 5) * -scaleY;//*(pTile + 5 + k * 5) / -127.f;//BYTE(255 - *(BYTE*)(pTile + 5 + k * 5) + 128) / 255.f;////
				vertices.push_back(vertex);
				vertex.texUV.x = 0.25f;
				vertex.position.m128_f32[0] += 0.5f;
				vertex.position.m128_f32[1] = *(pTile + 5 + k * 5 + 1) * -scaleY;//*(pTile + 5 + k * 5 + 1) / -127.f;//BYTE(255 - *(BYTE*)(pTile + 5 + k * 5 + 1) + 128) / 255.f;////
				vertices.push_back(vertex);
				vertex.texUV.x = 0.5f;
				vertex.position.m128_f32[0] += 0.5f;
				vertex.position.m128_f32[1] = *(pTile + 5 + k * 5 + 2) * -scaleY;//*(pTile + 5 + k * 5 + 2) / -127.f;//BYTE(255 - *(BYTE*)(pTile + 5 + k * 5 + 2) + 128) / 255.f;////
				vertices.push_back(vertex);
				vertex.texUV.x = 0.75f;
				vertex.position.m128_f32[0] += 0.5f;
				vertex.position.m128_f32[1] = *(pTile + 5 + k * 5 + 3) * -scaleY;//*(pTile + 5 + k * 5 + 3) / -127.f;//BYTE(255 - *(BYTE*)(pTile + 5 + k * 5 + 3) + 128) / 255.f;////
				vertices.push_back(vertex);
				vertex.texUV.x = 1.0f;
				vertex.position.m128_f32[0] += 0.5f;
				vertex.position.m128_f32[1] = *(pTile + 5 + k * 5 + 4) * -scaleY;//*(pTile + 5 + k * 5 + 4) / -127.f;//BYTE(255 - *(BYTE*)(pTile + 5 + k * 5 + 4) + 128) / 255.f;////
				vertices.push_back(vertex);
				vertex.position.m128_f32[2] += 0.5f;
			}

			for (int k = 0; k < 4; k++)
			{
				for (int l = 0; l < 4; l++)
				{
						//vertices[j * 25 + i * width * 25 + 5 * k + l].normal += XMVector3Cross((vertices[j * 25 + i * width * 25 + 5 * k + l + 1].position - vertices[j * 25 + i * width * 25 + 5 * k + l].position), (vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].position - vertices[j * 25 + i * width * 25 + 5 * k + l].position));
					vertices[j * 25 + i * width * 25 + 5 * k + l].normal = XMVector3Cross((vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].position - vertices[j * 25 + i * width * 25 + 5 * k + l].position), (vertices[j * 25 + i * width * 25 + 5 * k + l + 1].position - vertices[j * 25 + i * width * 25 + 5 * k + l].position));
					vertices[j * 25 + i * width * 25 + 5 * k + l + 1].normal = vertices[j * 25 + i * width * 25 + 5 * k + l].normal;
					vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].normal = vertices[j * 25 + i * width * 25 + 5 * k + l].normal;
					//vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].normal += XMVector3Cross((vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].position - vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].position), (vertices[j * 25 + i * width * 25 + 5 * k + l + 1].position - vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].position));
					vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].normal = XMVector3Cross((vertices[j * 25 + i * width * 25 + 5 * k + l + 1].position - vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].position), (vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].position - vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].position));
					vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].normal = vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].normal;
					vertices[j * 25 + i * width * 25 + 5 * k + l + 1].normal = vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].normal;
						//vertices[j * 25 + i * width * 25 + 5 * k + l + 1].normal += XMVector3Cross((vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].position - vertices[j * 25 + i * width * 25 + 5 * k + l + 1].position), (vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].position - vertices[j * 25 + i * width * 25 + 5 * k + l + 1].position));
						//vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l + 1].normal += vertices[j * 25 + i * width * 25 + 5 * k + l + 1].normal;
						//vertices[j * 25 + i * width * 25 + 5 * (k + 1) + l].normal += vertices[j * 25 + i * width * 25 + 5 * k + l + 1].normal;
				}
			}
			
		}
		
	}

	int lightSthCount = *(int*)(contents + offsetLightData);

	int terrainDiffuseColor = *(int*)(contents + offsetLightData + 4);
	int objectDiffuseColor = *(int*)(contents + offsetLightData + 8);

	//tile2Tex[0] = 3; //TEST
	level->setTerrainDiffuseColor((terrainDiffuseColor & 0xFF) / 255.f, (terrainDiffuseColor >> 8 & 0xFF) / 255.f, (terrainDiffuseColor >> 16 & 0xFF) / 255.f, (terrainDiffuseColor >> 24 & 0xFF) / 255.f);
	level->setObjectDiffuseColor((objectDiffuseColor & 0xFF) / 255.f, (objectDiffuseColor >> 8 & 0xFF) / 255.f, (objectDiffuseColor >> 16 & 0xFF) / 255.f, (objectDiffuseColor >> 24 & 0xFF) / 255.f);
	level->setTile2Tex(tile2Tex);
	level->setHeightmap(vertices, indices);
	level->setTerrainWorldMatrix(XMMatrixIdentity());

	return 0;
}

int LevelIO::readTEXT(ID3D11Device1* device, ID3D11DeviceContext1* deviceContext, Level* level, vector<char>* texContents, vector<char>* textContents)
{
	//char* contents = texContents->data();
	int numTextures = 0;
	int numMaterials = 0;
	char* tex = texContents->data();
	char* contents = textContents->data();

	char* pTex = contents;
	char* pPalette = contents + 0x800;

	//get the FUCKING number of actual textures
	while (*(short*)tex != -1)
	{
		if (numTextures == *(short*)tex)
			numTextures++;

		numMaterials++; //NOTE this is actually in the hmp
		tex += 4;
	}

	tex = texContents->data();

	if (numTextures)
	{
		BYTE* pixelData = new BYTE[64 * 64 * 4];
		unsigned int rowPitch;
		HRESULT hResult;

		level->setShaderResViews(new ID3D11ShaderResourceView * [numTextures]);
		level->setTextures(new ID3D11Texture2D * [numTextures]);

		level->setNumTextures(numTextures);

		//now do the thing
		for (int i = 0; i < numTextures; i++)
		{
			for (int j = 0; j < 2048; j++)
			{
				pixelData[j * 8] = pPalette[((pTex[j] & 0xF0) >> 4) * 3];
				pixelData[j * 8 + 1] = pPalette[((pTex[j] & 0xF0) >> 4) * 3 + 1];
				pixelData[j * 8 + 2] = pPalette[((pTex[j] & 0xF0) >> 4) * 3 + 2];
				pixelData[j * 8 + 3] = 0xFF;

				pixelData[j * 8 + 4] = pPalette[(pTex[j] & 0xF) * 3];
				pixelData[j * 8 + 5] = pPalette[(pTex[j] & 0xF) * 3 + 1];
				pixelData[j * 8 + 6] = pPalette[(pTex[j] & 0xF) * 3 + 2];
				pixelData[j * 8 + 7] = 0xFF;
			}

			D3D11_TEXTURE2D_DESC textureDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			textureDesc.Height = 64;
			textureDesc.Width = 64;
			textureDesc.MipLevels = 0;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

			rowPitch = 64 * 4;

			hResult = device->CreateTexture2D(&textureDesc, NULL, &level->getTextures()[i]);
			if (FAILED(hResult))
			{
				return 1;
			}

			deviceContext->UpdateSubresource(level->getTextures()[i], 0, NULL, pixelData, rowPitch, 0);

			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = -1;

			hResult = device->CreateShaderResourceView(level->getTextures()[i], &srvDesc, &level->getShaderResView()[i]);
			if (FAILED(hResult))
			{
				return 1;
			}

			// Generate mipmaps for this texture.
			deviceContext->GenerateMips(level->getShaderResView()[i]);

			pTex += 0x830;
			pPalette += 0x830;
		}

		delete[] pixelData;
	}
	else {
		return 1;
	}

	//update tex indices
	for (int i = 0; i < level->getTileCount(); i++)
	{
		level->getTile2Tex()[i] = *(uint16_t*)(tex + level->getTile2Tex()[i] * 4);
	}

	return 0;
}

int LevelIO::writeHmpToBitmap(const char* name, vector<char>* fileContents)
{
	char* contents = fileContents->data();

	uint16_t width = *(uint16_t*)(contents + 0x24);
	uint16_t height = *(uint16_t*)(contents + 0x26);

	uint16_t widthPixels = (width - 1) * 4 + 5;
	uint16_t heightPixels = (height - 1) * 4 + 5;

	//array<BYTE, widthPixels * heightPixels * 3> pixelDataTest;
	BYTE* pixelData = new BYTE[widthPixels * heightPixels * 3]; //24 bit
	BYTE* pixelDataOutput = new BYTE[widthPixels * heightPixels * 3 + heightPixels];

	int offsetTileData = *(int*)(contents + 0x1C);
	char* pTileData = contents + offsetTileData;
	char* pTiles = contents + 0x28;
	char* pTile = 0;

	int row = -1;
	int column = 0;
	int index = 0;
	uint16_t tileDataIdx = 0;
	bool BULLSHIT = false;

	for (int i = 0; i < width * height; i++)
	{
		if (!(i % width))
		{
			row++;
			column = 0;
		}

		tileDataIdx = *(uint16_t*)(pTiles + i * 2);
		pTile = pTileData + tileDataIdx * 0x1E;

		for (int j = 0; j < 5; j++)
		{
			index = row * widthPixels * 4 * 3 + column * 4 * 3 + j * widthPixels * 3;

			pixelData[index] = 255 - *(BYTE*)(pTile + 5 + j * 5) + 128;
			pixelData[index + 1] = 255 - *(BYTE*)(pTile + 5 + j * 5) + 128;
			pixelData[index + 2] = 255 - *(BYTE*)(pTile + 5 + j * 5) + 128;

			pixelData[index + 3] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 1) + 128;
			pixelData[index + 4] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 1) + 128;
			pixelData[index + 5] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 1) + 128;

			pixelData[index + 6] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 2) + 128;
			pixelData[index + 7] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 2) + 128;
			pixelData[index + 8] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 2) + 128;

			pixelData[index + 9] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 3) + 128;
			pixelData[index + 10] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 3) + 128;
			pixelData[index + 11] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 3) + 128;

			pixelData[index + 12] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 4) + 128;
			pixelData[index + 13] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 4) + 128;
			pixelData[index + 14] = 255 - *(BYTE*)(pTile + 5 + j * 5 + 4) + 128;
		}

		column++;
	}

	for (int i = 0; i < heightPixels; i++)
	{
		memcpy(pixelDataOutput + i * widthPixels * 3 + i, pixelData + i * widthPixels * 3, widthPixels * 3);
	}

	filebuf fb;
	fb.open(name, ios::out | ios::binary);
	ostream os(&fb);

	BITMAPFILEHEADER bmfHeader;
	bmfHeader.bfType = 0x4D42;
	bmfHeader.bfSize = 0x36 + widthPixels * heightPixels * 3 + heightPixels;
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfOffBits = 0x36;

	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = 0x28;
	bmiHeader.biWidth = widthPixels;
	bmiHeader.biHeight = heightPixels;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 0x18;
	bmiHeader.biCompression = 0;
	bmiHeader.biSizeImage = widthPixels * heightPixels * 3 + heightPixels;
	bmiHeader.biXPelsPerMeter = 0xB12;
	bmiHeader.biYPelsPerMeter = 0xB12;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;

	os.write((char*)&bmfHeader, sizeof(BITMAPFILEHEADER));
	os.write((char*)&bmiHeader, sizeof(BITMAPINFOHEADER));
	os.write((char*)pixelDataOutput, widthPixels * heightPixels * 3 + heightPixels);

	fb.close();
	delete[] pixelData;
	delete[] pixelDataOutput;

	return 0;
}