#ifndef _TERRAINLITSHADER_H_
#define _TERRAINLITSHADER_H_

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

#include "ModelCommonTypes.h"

using namespace DirectX;
using namespace std;

class TerrainLitShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMVECTOR diffuseColor;
		XMVECTOR lightDirection;
	};

public:

	TerrainLitShader();
	TerrainLitShader(const TerrainLitShader&);
	~TerrainLitShader();

	bool Initialize(ID3D11Device1*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX, uint16_t, uint16_t, ID3D11ShaderResourceView**, uint16_t*, XMVECTOR, XMVECTOR);

private:
	bool InitializeShader(ID3D11Device1*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX, XMVECTOR, XMVECTOR);
	void RenderShader(ID3D11DeviceContext1*, unsigned int, ID3D11ShaderResourceView**, uint16_t);


private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
};

#endif