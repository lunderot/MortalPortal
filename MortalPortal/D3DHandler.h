#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <stdexcept>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class D3DHandler
{

private:

	IDXGISwapChain*				swapChain;
	ID3D11Device*			    device;
	ID3D11DeviceContext*		deviceContext;
	ID3D11RenderTargetView*		renderTargetView;

	//Used for 2D rendering, ex. text.
	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11DepthStencilState*	depthStencilState;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11RasterizerState*		rasterState;
	ID3D11DepthStencilState*    depthDisabledStencilState;
	ID3D11BlendState*			alphaEnableBlendingState;
	ID3D11BlendState*			alphaDisableBlendingState;

	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 orthoMatrix;

public:

	D3DHandler(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	~D3DHandler();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(DirectX::XMFLOAT4X4& projectionMatrix);
	void GetWorldMatrix(DirectX::XMFLOAT4X4& worldMatrix);
	void GetOrthoMatrix(DirectX::XMFLOAT4X4& orthoMatrix);
};

