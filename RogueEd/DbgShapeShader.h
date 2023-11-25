#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

#include "ModelCommonTypes.h"

using namespace DirectX;
using namespace std;

class DbgShapeShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ColorBufferType
	{
		XMVECTOR color;
	};

public:

	DbgShapeShader();
	DbgShapeShader(const DbgShapeShader&);
	~DbgShapeShader();

	bool Initialize(ID3D11Device1*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext1*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMVECTOR);

private:
	bool InitializeShader(ID3D11Device1*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX, XMVECTOR);
	void RenderShader(ID3D11DeviceContext1*, int);


private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_colorBuffer, * m_matrixBuffer;
};