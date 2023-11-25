#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace std;
using namespace DirectX;


class FontShader
{
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct PixelBufferType
	{
		XMVECTOR pixelColor;
	};

public:
	FontShader();
	FontShader(const FontShader&);
	~FontShader();

	bool Initialize(ID3D11Device1*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext1*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMVECTOR);

private:
	bool InitializeShader(ID3D11Device1*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext1*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView* texture, XMVECTOR);
	void RenderShader(ID3D11DeviceContext1*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_constantBuffer;
	ID3D11Buffer* m_colorBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_pixelBuffer;

	
};

