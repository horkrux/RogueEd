#include "Graphics.h"

using namespace DirectX;

RogueObject* rogueTest;
RogueObject* rogueTest2;
RogueObject* rogueTest3;
RogueObject* rogueTestSky;
HOB* skyHOB;
Level* testLevel;
DbgShapeBox* testBox;
DbgShapeBox* testBox2;
DbgShapeBox* testBox3;
XMMATRIX shitViewMatrix;

Graphics::Graphics()
{
	m_objectIndex = 0;
	m_modelChange = false;
	m_Direct3D = 0;
	m_TestShader = 0;
	m_TerrainShader = 0;
	m_TestCamera = 0;
	m_TestHOB = 0;
	m_TestLevel = 0;
	m_TestText = 0;
}

Graphics::Graphics(const Graphics& other){}
Graphics::~Graphics(){}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Direct3D = new D3DSystem;
	if (!m_Direct3D)
	{
		return false;
	}

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_TestCamera = new TestCamera;
	if (!m_TestCamera)
	{
		return false;
	}

	m_TestShader = new TestShader;
	if (!m_TestShader)
	{
		return false;
	}

	if (!m_TestShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	m_fontShader = new FontShader;
	if (!m_fontShader)
	{
		return false;
	}

	if (!m_fontShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	m_TerrainShader = new TerrainLitShader();
	if (!m_TerrainShader)
	{
		return false;
	}

	if (!m_TerrainShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	m_DbgShapeShader = new DbgShapeShader;
	if (!m_DbgShapeShader)
	{
		return false;
	}

	if (!m_DbgShapeShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the debug shader object.", L"Error", MB_OK);
		return false;
	}
	// Set the initial position of the camera.
	//m_TestCamera->SetPosition(0.0f, 0.0f, -5.0f);
	//m_TestCamera->SetPosition(108.9434f + 1.0f, -4.95f, 112.3335f + 1.0f);
	m_TestCamera->SetPosition(0, 0, -5.f);
	m_TestCamera->Render();
	m_TestCamera->GetViewMatrix(shitViewMatrix);
	//DataIO* dataIO = DataIO::getInstance();
	////dataIO->read("DATA.HDR");
	//vector<char> filecontents;
	//vector<char> hobfilecontents;
	//vector<char> hmtfilecontents;
	//vector<char> tiehobfilecontents;
	//vector<char> tiehmtfilecontents;
	//vector<char> skyhobfilecontents;
	//vector<char> skyhmtfilecontents;
	//if (dataIO->getFileContents("data/level/lv_0/hmp", &filecontents))
	//	OutputDebugStringA("whoops\n");
	//if (dataIO->getFileContents("data/imp_stuff/tiebomber_HOB", &hobfilecontents))
	//	OutputDebugStringA("hehe\n");
	//if (dataIO->getFileContents("data/imp_stuff/tiebomber_HMT", &hmtfilecontents))
	//	OutputDebugStringA("hoho\n");
	//if (dataIO->getFileContents("data/imp_stuff/atat_HOB", &tiehobfilecontents))
	//	OutputDebugStringA("TIE HOB FAILED\n");
	//if (dataIO->getFileContents("data/imp_stuff/atat_HMT", &tiehmtfilecontents))
	//	OutputDebugStringA("TIE HMT FAILED\n");
	//if (dataIO->getFileContents("data/level/lv_1/1ky_HOB", &skyhobfilecontents))
	//	OutputDebugStringA("SKY HOB FAILED\n");
	//if (dataIO->getFileContents("data/level/lv_1/1ky_HMT", &skyhmtfilecontents))
	//	OutputDebugStringA("SKY HMT FAILED\n");

	//m_TestHOB = new HOB();
	//m_TestHOB->initializeSHIT(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), tiehobfilecontents, tiehmtfilecontents);

	//rogueTest = new RogueObject();
	//rogueTest->setHobReference(m_TestHOB);
	//rogueTest->setType(ATAT);

	//addRenderObject(rogueTest);
	//m_TestHOB2 = new HOB();
	//m_TestHOB2->initializeSHIT(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hobfilecontents, hmtfilecontents);

	//rogueTest2 = new RogueObject();
	//rogueTest2->setHobReference(m_TestHOB2);
	//rogueTest2->setEventId(1);
	//rogueTest2->setType(TIE_BOMBER);
	////delete m_TestHOB2;
	//XMMATRIX transMatrix = XMMatrixTranslationFromVector(XMVectorSet(10.0f, 0.0f, 0.0f, 1.0f));
	//rogueTest2->setWorldMatrix(transMatrix);

	//addRenderObject(rogueTest2);

	//rogueTest3 = new RogueObject();
	//rogueTest3->setHobReference(m_TestHOB2);
	//rogueTest3->setEventId(2);
	//rogueTest3->setType(TIE_BOMBER);

	//addRenderObject(rogueTest3);

	//skyHOB = new HOB();
	//skyHOB->initializeSHIT(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), skyhobfilecontents, skyhmtfilecontents);

	//rogueTestSky = new RogueObject();
	//rogueTestSky->setHobReference(skyHOB);
	//
	//XMMATRIX scaleMatrix = XMMatrixScalingFromVector(XMVectorSet(1000.0f, 1000.0f, 1000.0f, 1.0f));
	//rogueTestSky->setWorldMatrix(scaleMatrix);

	//addRenderObject(rogueTestSky);
	vector<char> lvlContents;
	vector<char> hmpContents;

	vector<char> texContents;
	vector<char> textContents;

	LevelIO* levelIO = LevelIO::getInstance();
	m_TestLevel = new Level();
	m_TestLevel->setLevelId('0');

	DataIO* dataIO = DataIO::getInstance();
	//dataIO->read("DATA.HDR");
	dataIO->getFileContents("data/level/lv_0/dat", &lvlContents);
	dataIO->getFileContents("data/level/lv_0/hmp", &hmpContents);
	dataIO->getFileContents("data/level/lv_0_TEX", &texContents);
	dataIO->getFileContents("data/level/lv_0_TEXT", &textContents);

	levelIO->readDat(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_TestLevel, &lvlContents);
	levelIO->readHmp(m_TestLevel, &hmpContents);
	levelIO->readTEXT(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_TestLevel, &texContents, &textContents);

	m_TestLevel->initializeHeightmap(m_Direct3D->GetDevice());
	//testBox = new DbgShapeBox(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	XMMATRIX worldMatrixTerrain = XMMatrixTranslation(1.0f, 0.0f/*terrainPositionY*/, 1.0f);
	worldMatrixTerrain *= XMMatrixScaling(1.0f, 1.0f, 1.0f);//1.25f
	

	m_TestLevel->setTerrainWorldMatrix(worldMatrixTerrain);

	XMMATRIX textMatrix = XMMatrixTranslation(10.f, 5.f, 10.f);


	//m_TestText = new Text(m_Direct3D->GetDevice());
	//m_TestText->setWorldMatrix(textMatrix);
	// Create the text object.
	m_TestText = new Text;
	if (!m_TestText)
	{
		return false;
	}

	// Initialize the text object.
	result = m_TestText->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, shitViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	
	testBox = new DbgShapeBox(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	XMMATRIX m_boxworldMatrix = XMMatrixRotationRollPitchYaw(0 * 0.0174532925f, 50.f * 0.0174532925f, 0 * 0.0174532925f);
	m_boxworldMatrix *= XMMatrixScaling(10.806f * 0.2142857, 5.137f * 0.2142857, 10.04f * 0.2142857);//6.843f * 0.2142857, 2.41f * 0.2142857, 11.476f * 0.2142857);//1.2f, 1.0f, 2.2f);
	m_boxworldMatrix *= XMMatrixTranslation(27.3661f, -4.375f, 104.7895f);// - 5.738159f / 4, -5.f, 116.3636f + 3.421645f / 4);
	testBox->setColor(XMVectorSet(1.0f, 0, 0, 1.0f));
	testBox->setWorldMatrix(m_boxworldMatrix);

	testBox2 = new DbgShapeBox(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	m_boxworldMatrix = XMMatrixRotationRollPitchYaw(0 * 0.0174532925f, 50.f * 0.0174532925f, 0 * 0.0174532925f);
	m_boxworldMatrix *= XMMatrixScaling(19.28f * 0.2142857, 5.14f * 0.2142857, 17.81f * 0.2142857);//1.2f, 1.0f, 2.2f);
	m_boxworldMatrix *= XMMatrixTranslation(27.3661f, -4.375f + 4.67f * 0.2142857, 104.7895f);// - 5.738159f / 4, -5.f, 116.3636f + 3.421645f / 4);
	testBox2->setColor(XMVectorSet(1.0f, 0, 0, 1.0f));
	testBox2->setWorldMatrix(m_boxworldMatrix);

	testBox3 = new DbgShapeBox(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	m_boxworldMatrix = XMMatrixRotationRollPitchYaw(0 * 0.0174532925f, 50.f * 0.0174532925f, 0 * 0.0174532925f);
	m_boxworldMatrix *= XMMatrixScaling(28.1f * 0.2142857, 3.92f * 0.2142857, 26.78f * 0.2142857);//1.2f, 1.0f, 2.2f);
	m_boxworldMatrix *= XMMatrixTranslation(27.3661f, -4.375f, 104.7895f);// - 5.738159f / 4, -5.f, 116.3636f + 3.421645f / 4);
	testBox3->setColor(XMVectorSet(1.0f, 0, 0, 1.0f));
	testBox3->setWorldMatrix(m_boxworldMatrix);

	//levelIO->writeHmpToBitmap("lv_j.bmp", &hmpContents);
	//m_TestHMT = new HMT();
	//m_TestHMT->initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hmtfilecontents);
}

void Graphics::Shutdown()
{
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	if (m_TestHOB)
	{
		delete m_TestHOB;
		m_TestHOB = 0;
	}

	if (m_TestHOB2)
	{
		delete m_TestHOB2;
		m_TestHOB2 = 0;
	}

	if (m_TestLevel)
	{
		delete m_TestLevel;
		m_TestLevel = 0;
	}

	if (m_TestShader)
	{
		m_TestShader->Shutdown();
		m_TestShader = 0;
	}

	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		m_TerrainShader = 0;
	}

	if (m_fontShader)
	{
		m_fontShader->Shutdown();
		m_fontShader = 0;
	}

	if (m_DbgShapeShader)
	{
		m_DbgShapeShader->Shutdown();
		m_DbgShapeShader = 0;
	}

	if (testBox)
	{
		delete testBox;
		testBox = 0;
	}

	if (testBox2)
	{
		delete testBox2;
		testBox2 = 0;
	}

	if (testBox3)
	{
		delete testBox3;
		testBox3 = 0;
	}
}

void Graphics::ChangeMap(char mapId)
{
	if (m_TestLevel)
	{
		delete m_TestLevel;
		m_TestLevel = 0;
	}

	vector<char> lvlContents;
	vector<char> hmpContents;

	vector<char> texContents;
	vector<char> textContents;

	LevelIO* levelIO = LevelIO::getInstance();
	m_TestLevel = new Level();
	m_TestLevel->setLevelId(mapId);

	DataIO* dataIO = DataIO::getInstance();
	//dataIO->read("DATA.HDR");

	char pathDat[32];
	char pathHmp[32];
	char pathTex[32];
	char pathText[32];

	sprintf_s(pathDat, 32, "data/level/lv_%c/dat", mapId);
	sprintf_s(pathHmp, 32, "data/level/lv_%c/hmp", mapId);
	sprintf_s(pathTex, 32, "data/level/lv_%c_TEX", mapId);
	sprintf_s(pathText, 32, "data/level/lv_%c_TEXT", mapId);
	
	dataIO->getFileContents(pathDat, &lvlContents);
	dataIO->getFileContents(pathHmp, &hmpContents);
	dataIO->getFileContents(pathTex, &texContents);
	dataIO->getFileContents(pathText, &textContents);

	levelIO->readDat(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_TestLevel, &lvlContents);
	levelIO->readHmp(m_TestLevel, &hmpContents);
	levelIO->readTEXT(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_TestLevel, &texContents, &textContents);

	m_TestLevel->initializeHeightmap(m_Direct3D->GetDevice());
	//testBox = new DbgShapeBox(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	XMMATRIX worldMatrixTerrain = XMMatrixTranslation(1.0f, 0.0f/*terrainPositionY*/, 1.0f);
	worldMatrixTerrain *= XMMatrixScaling(1.0f, 1.0f, 1.0f);//1.25f


	m_TestLevel->setTerrainWorldMatrix(worldMatrixTerrain);

	

}

bool Graphics::Frame(XMFLOAT3 camPosition, XMFLOAT3 camRotation, float terrainPositionY)
{
	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_TestCamera->SetPosition(camPosition.x, camPosition.y, camPosition.z);
	m_TestCamera->SetRotation(camRotation.x, camRotation.y, camRotation.z);

	result = Render(rotation, terrainPositionY);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render(float rotation, float terrainPositionY)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	//m_TestCamera->SetPosition(0.0f, 0.0f, -0.50f);
	m_TestCamera->Render();
	
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_TestCamera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//worldMatrix *= XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	
	/*if (m_modelChange)
	{
		if (m_TestHOB)
		{
			m_TestHOB->~HOB();
			m_TestHOB = 0;
		}


	}*/

	//int objectCount = m_TestHOB->getNumObjects();
	//DrawInfo* drawInfo = new DrawInfo;
	int objectIndex = 0;
	
	/*for (int i = 0; i < m_TestHOB->getNumObjects(); i++)
	{
		if (!strcmp("sphere", m_TestHOB->getObjects()[i].name))
		{
			objectIndex = i;
			break;
		}
	}*/
	objectIndex = m_objectIndex;
	//objectIndex = 2;
	/*if (m_TestHOB->getNumObjects())
	{
		m_TestHOB->render(m_Direct3D->GetDeviceContext(), objectIndex);

		result = m_TestShader->Render(m_Direct3D->GetDeviceContext(), m_TestHOB->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_TestHOB->getObjects()[objectIndex].drawInfo, m_TestHOB->getMaterialBundle()->getShaderResViews(), m_TestHOB->getMaterialBundle()->getMat2TexIndices());

		if (!result)
		{
			return false;
		}
	}*/
	XMFLOAT3 rot = m_TestCamera->GetRotation();
	//int shit = m_TestLevel->getNumObjects();
	//unordered_multimap<RogueObjectType, RogueObject*>* FUCK = m_TestLevel->getRenderList();
	unordered_multimap<RogueObjectType, RogueObject*>::iterator it = m_TestLevel->getRenderList()->begin();
	unsigned int modelId = 0;
	RogueObjectType type = DEFAULT;

	XMFLOAT3 cameraPosition = m_TestCamera->GetPosition();
	XMMATRIX worldMatrixText = XMMatrixIdentity();//m_TestText->getWorldMatrix();
	float angle = atan2(10.f - cameraPosition.x, 10.f - cameraPosition.z) * (180.0 / XM_PI);
	float rotation2 = angle * 0.0174532925f;

	worldMatrixText = XMMatrixTranslation(10.f, 5.f, 10.f);
	worldMatrixText *= XMMatrixRotationY(rotation2);

	//m_TestText->setWorldMatrix(worldMatrixText);

	worldMatrixText = XMMatrixIdentity();//m_TestText->getWorldMatrix();
	worldMatrixText *= viewMatrix;
	worldMatrixText *= projectionMatrix;
	XMVECTOR scale;
	XMVECTOR translationText;
	XMVECTOR rotationText;
	XMMatrixDecompose(&scale, &rotationText, &translationText, worldMatrixText);
	XMFLOAT4 translation;
	XMStoreFloat4(&translation, translationText);
	translation.x /= translation.w;
	translation.y /= translation.w;
	translation.z /= translation.w;
	//translation.x = (translation.x + 1.f) / 2.f;
	//translation.y = (translation.y + 1.f) / 2.f;
	//translation.z = (translation.z + 1.f) / 2.f;
	bool lolwtfamidoing = false;
	if (translation.w < 0.0f)
	{
		lolwtfamidoing = true;
	}
	translation.x = 800 / 2 * translation.x;
	translation.y = 600 / 2 * translation.y;
	translation.z = 0;
	translation.w = 1.0f;
	XMMATRIX shit = XMMatrixTranslation(translation.x, translation.y, translation.z);
	worldMatrixText = shit;
	XMVECTOR fuck = XMVectorSet(0, 0, 0, 1);
	fuck = XMVector4Transform(fuck, worldMatrixText);
	fuck = XMVector4Transform(fuck, shit);
	fuck = XMVector4Transform(fuck, orthoMatrix);
	

	while (it != m_TestLevel->getRenderList()->end())
	{
		
		if (type == DEFAULT || type != it->first || type == NON_DESTRUCTIBLE || type == GENERIC_DESTROYABLE)
		{
			type = it->first;
			it->second->getHobReference()->render(m_Direct3D->GetDeviceContext(), it->second->getModelId());
		}
		modelId = it->second->getModelId();

		result = m_TestShader->Render(m_Direct3D->GetDeviceContext(), 0, it->second->getWorldMatrix(), viewMatrix, projectionMatrix, it->second->getHobReference()->getObjects()[modelId].drawInfo, it->second->getHobReference()->getMaterialBundle()->getShaderResViews(), it->second->getHobReference()->getMaterialBundle()->getMat2TexIndices(), true);

		if (!result)
			return false;

		it++;
	}

	m_TestLevel->renderHeightmap(m_Direct3D->GetDeviceContext());
	//-1.60999906 gets us close
	//-1.4 now
	//6.33998394 new
	//6.35

	//result = m_TestShader->RenderTerrain(m_Direct3D->GetDeviceContext(), XMMatrixScaling(1.0f, m_TestLevel->getTerrainScaleY() * 127/*6.35f*/, 1.0f) * m_TestLevel->getTerrainWorldMatrix(), viewMatrix, projectionMatrix, m_TestLevel->getWidth(), m_TestLevel->getHeight(), m_TestLevel->getShaderResView(), m_TestLevel->getTile2Tex());
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_TestLevel->getTerrainWorldMatrix(), viewMatrix, projectionMatrix, m_TestLevel->getWidth(), m_TestLevel->getHeight(), m_TestLevel->getShaderResView(), m_TestLevel->getTile2Tex(), XMVectorSet(1.f, -7.45f, 0.f, 1.f), m_TestLevel->getTerrainDiffuseColor());

	if (!result)
		return false;

	//testBox->render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	DbgShape* shape;

	for (int i = 0; i < m_TestLevel->getDbgRenderList().size(); i++)
	{
		shape = m_TestLevel->getDbgRenderList()[i];

		if (shape->getShapeType() == DbgShape::ShapeType::BOX)
		{
			((DbgShapeBox*)shape)->getReferenceBox()->Render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

			result = m_DbgShapeShader->Render(m_Direct3D->GetDeviceContext(), shape->getIndexCount(), shape->getWorldMatrix(), viewMatrix, projectionMatrix, shape->getColor());

			if (!result)
				return false;
		} 
		else
		{
			((DbgShapeSphere*)shape)->getReferenceSphere()->Render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

			result = m_DbgShapeShader->Render(m_Direct3D->GetDeviceContext(), ((DbgShapeSphere*)shape)->getReferenceSphere()->getIndexCount(), shape->getWorldMatrix(), viewMatrix, projectionMatrix, shape->getColor());
			
			//m_TestLevel->getDbgRenderList()[i]->render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext()); //TODO: only call this once, put color into pixel shader constant

			//result = m_DbgShapeShader->Render(m_Direct3D->GetDeviceContext(), m_TestLevel->getDbgRenderList()[i]->getIndexCount(), m_TestLevel->getDbgRenderList()[i]->getWorldMatrix(), viewMatrix, projectionMatrix, m_TestLevel->getDbgRenderList()[i]->getColor());

			if (!result)
				return false;
		}
		//if (m_TestLevel->getDbgRenderList()[i]->getEventId() == 0x16)
		//if (m_TestLevel->getDbgRenderList()[i]->getName() == "convoy1" || m_TestLevel->getDbgRenderList()[i]->getEventId() == 0x53)
		//{
			
		//}
		
	}

	testBox->render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	result = m_DbgShapeShader->Render(m_Direct3D->GetDeviceContext(), testBox->getIndexCount(), testBox->getWorldMatrix(), viewMatrix, projectionMatrix, testBox->getColor());

	if (!result)
		return false;

	testBox2->render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	result = m_DbgShapeShader->Render(m_Direct3D->GetDeviceContext(), testBox2->getIndexCount(), testBox2->getWorldMatrix(), viewMatrix, projectionMatrix, testBox2->getColor());

	if (!result)
		return false;

	testBox3->render(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());
	result = m_DbgShapeShader->Render(m_Direct3D->GetDeviceContext(), testBox3->getIndexCount(), testBox3->getWorldMatrix(), viewMatrix, projectionMatrix, testBox3->getColor());

	if (!result)
		return false;

	//result = m_TestShader->RenderDbgShape(m_Direct3D->GetDeviceContext(), reinterpret_cast<RogueObject*>(testBox)->getWorldMatrix(), viewMatrix, projectionMatrix);

	//if (!result)
	//	return false;
	/*for (int i = 0; i < 1000; i++)
	{
		if (m_renderList[i])
		{
			m_renderList[i]->getHobReference()->render(m_Direct3D->GetDeviceContext(), m_renderList[i]->getModelId());

			result = m_TestShader->Render(m_Direct3D->GetDeviceContext(), 0, m_renderList[i]->getWorldMatrix(), viewMatrix, projectionMatrix, m_renderList[i]->getHobReference()->getObjects()[m_renderList[i]->getModelId()].drawInfo, m_renderList[i]->getHobReference()->getMaterialBundle()->getShaderResViews(), m_renderList[i]->getHobReference()->getMaterialBundle()->getMat2TexIndices());

			if (!result)
			{
				return false;
			}
		}
	}*/
	/*m_TestHOB->render(m_Direct3D->GetDeviceContext(), 1);

	for (int i = 0; i < m_TestHOB->getNumObjects(); i++)
	{
		if (!strcmp("atstdest", m_TestHOB->getObjects()[i].name))
		{
			objectIndex = i;
			break;
		}
	}*/

	//result = m_TestShader->Render(m_Direct3D->GetDeviceContext(), m_TestHOB->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_TestHOB->getObjects()[objectIndex].drawInfo, m_TestHOB->getMaterialBundle()->getShaderResViews());

	/*if (!result)
	{
		return false;
	}*/
	// Present the rendered scene to the screen.
	//if (!lolwtfamidoing)
	{
		m_Direct3D->GetWorldMatrix(shit);

		m_Direct3D->TurnOnAlphaBlending();
		m_Direct3D->TurnZBufferOff();

		result = m_TestText->Render(m_Direct3D->GetDeviceContext(), shit, orthoMatrix);
		if (!result)
		{
			return false;
		}

		//m_TestText->Render(m_Direct3D->GetDeviceContext());
		//m_fontShader->Render(m_Direct3D->GetDeviceContext(), 4, /*m_TestText->getWorldMatrix()*/shit, shitViewMatrix, orthoMatrix);

		m_Direct3D->TurnOffAlphaBlending();
		m_Direct3D->TurnZBufferOn();

	}
	m_Direct3D->EndScene();

	return true;
}

void Graphics::addRenderObject(RogueObject* obj)
{
	m_renderList.insert({ obj->getType(), obj });
	//m_renderList[obj->getEventId()] = obj;
}

void Graphics::removeRenderObject(RogueObjectType key, unsigned int eventId)
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
	//m_renderList.at(modelId)
	//m_renderList[obj->getEventId()] = NULL;
}

void Graphics::initModelChange()
{
	m_modelChange = true;
}

void Graphics::setCurrentModel(vector<char> hobfilecontents, vector<char> hmtfilecontents)
{
	if (m_TestHOB)
	{
		delete m_TestHOB;
		m_TestHOB = 0;
	}
	m_TestHOB = new HOB();
	m_TestHOB->initializeSHIT(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hobfilecontents, hmtfilecontents);
}

void Graphics::setCurrentObjectIndex(int objectIndex)
{
	m_objectIndex = objectIndex;
}