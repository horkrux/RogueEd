#ifndef _D3DSYSTEM_H_
#define _D3DSYSTEM_H_

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d2d1.h>
#include <d3d11_1.h>
#include <directxmath.h>
using namespace DirectX;

class D3DSystem
{
public:
	D3DSystem();
	D3DSystem(const D3DSystem&);
	~D3DSystem();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device1* GetDevice();
	ID3D11DeviceContext1* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOff();
	void TurnZBufferOn();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11Device1* m_device1;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11DeviceContext1* m_deviceContext1;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilView* m_depthStencilView;

	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	ID3D11RasterizerState* m_rasterState;
	ID2D1Factory* m_pD2DFactory;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif