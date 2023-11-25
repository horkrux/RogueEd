#pragma once

#include <vector>
#include <d3d11_1.h>
#include <directxmath.h>
using namespace DirectX;

#include "ModelCommonTypes.h"
#include "Font.h"
#include "FontShader.h"

class Text
{
private:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer, * indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMVECTOR position;
		XMFLOAT2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Initialize(ID3D11Device1*, ID3D11DeviceContext1*, HWND, int, int, XMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX);
	bool InitializeSentence(SentenceType**, int, ID3D11Device1*);
	//bool SetMousePosition(int, int, ID3D11DeviceContext*);
	//bool SetFps(int, ID3D11DeviceContext*);
	//bool SetCpu(int, ID3D11DeviceContext*);

private:
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext1*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext1*, SentenceType*, XMMATRIX, XMMATRIX);

private:
	Font* m_Font;
	FontShader* m_FontShader;
	int m_screenWidth, m_screenHeight;
	XMMATRIX m_baseViewMatrix;
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;
};