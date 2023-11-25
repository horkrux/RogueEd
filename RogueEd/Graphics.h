#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>

#include "D3DSystem.h"
#include "DataIO.h"
#include "TestCamera.h"
#include "TestShader.h"
#include "TerrainLitShader.h"
#include "FontShader.h"
#include "DbgShapeShader.h"
#include "HOB.h"
#include "RogueObject.h"
#include "LevelIO.h"
#include "Text.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();
	
	bool Initialize(int, int, HWND);
	void Shutdown();
	void ChangeMap(char);
	bool Frame(XMFLOAT3, XMFLOAT3, float);
	void initModelChange();
	void setCurrentModel(vector<char>, vector<char>);
	void setCurrentObjectIndex(int);

	void addRenderObject(RogueObject*);
	void removeRenderObject(RogueObjectType, unsigned int);

private:
	bool Render(float, float);

	D3DSystem* m_Direct3D;
	TestCamera* m_TestCamera;
	TestShader* m_TestShader;
	TerrainLitShader* m_TerrainShader;
	DbgShapeShader* m_DbgShapeShader;
	FontShader* m_fontShader;
	int m_objectIndex;
	bool m_modelChange;
	HOB* m_TestHOB;
	HOB* m_TestHOB2;
	HMT* m_TestHMT;
	Level* m_TestLevel;
	Text* m_TestText;

	unordered_multimap<RogueObjectType, RogueObject*> m_renderList;
};


#endif