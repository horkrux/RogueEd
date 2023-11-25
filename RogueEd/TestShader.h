#ifndef _TESTSHADER_H_
#define _TESTSHADER_H_

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

#include "ModelCommonTypes.h"

using namespace DirectX;
using namespace std;

class TestShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:

	TestShader();
	TestShader(const TestShader&);
	~TestShader();

	bool Initialize(ID3D11Device1*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext1*, int, XMMATRIX, XMMATRIX, XMMATRIX, const DrawInfo&, ID3D11ShaderResourceView**, int*, bool);
	bool RenderTerrain(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX, uint16_t, uint16_t, ID3D11ShaderResourceView**, uint16_t*);
	bool RenderDbgShape(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device1*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext1*, const DrawInfo&, ID3D11ShaderResourceView**, int*, bool);
	//void RenderTerrain(ID3D11DeviceContext1*, unsigned int, ID3D11ShaderResourceView**, uint16_t);
	

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11SamplerState* m_sampleStateClamp;
};

#endif
