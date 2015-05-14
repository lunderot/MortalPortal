#include "D3DHandler.h"

using namespace DirectX;
using namespace std;

D3DHandler::D3DHandler(int screenWidth, int screenHeight, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	swapChain = nullptr;
	device = nullptr;
	deviceContext = nullptr;
	renderTargetView = nullptr;
	depthStencilBuffer = nullptr;
	depthStencilState = nullptr;
	depthStencilView = nullptr;
	rasterState = nullptr;
	depthDisabledStencilState = nullptr;
	alphaFewOverlappingBlendingState = nullptr;
	alphaSeveralOverlappingBlendingState = nullptr;
	alphaDisableBlendingState = nullptr;


	HRESULT result;

	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspectRatio;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;


	fieldOfView = XM_PI / 4.0f;
	screenAspectRatio = (float)screenWidth / (float)screenHeight;


	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, device, and device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);
	
	// FOR DEBUGGING PURPOSES ONLY
	//result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_SOFTWARE, LoadLibrary(L"d3d11ref.dll"), 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	if (FAILED(result))
	{
		throw std::runtime_error("Could not create swap buffer");
	}

	//Get the pointer to the back buffer.
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		throw std::runtime_error("Could not get swap chain pointer");
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if (FAILED(result))
	{
		throw std::runtime_error("CreateRenderTargetView error");
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth buffer error");
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth stencil error");
	}
	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth stencil view error");
	}

	//Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//Create the rasterizer state from the description
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);
	if (FAILED(result))
	{
		throw std::runtime_error("Resterizer state error");
	}
	deviceContext->RSSetState(rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);
	
	// Create the projection matrix for 3D rendering.
	XMStoreFloat4x4(&projectionMatrix, XMMatrixPerspectiveFovLH(fieldOfView, screenAspectRatio, screenNear, screenDepth));
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());

	// Create an orthographic projection matrix for 2D rendering.
	XMStoreFloat4x4(&orthoMatrix, XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth));

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = device->CreateDepthStencilState(&depthDisabledStencilDesc, &depthDisabledStencilState);
	if (FAILED(result))
	{
		throw std::runtime_error("Depth stencil state error");
	}


	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDescription.AlphaToCoverageEnable = false;
	blendStateDescription.IndependentBlendEnable = false;

	result = device->CreateBlendState(&blendStateDescription, &alphaFewOverlappingBlendingState);
	if (FAILED(result))
	{
		throw std::runtime_error("Blend state error");
	}
	blendStateDescription.AlphaToCoverageEnable = true;
	result = device->CreateBlendState(&blendStateDescription, &alphaSeveralOverlappingBlendingState);

	if (FAILED(result))
	{
		throw std::runtime_error("Blend state error 2");
	}

	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	result = device->CreateBlendState(&blendStateDescription, &alphaDisableBlendingState);
	if (FAILED(result))
	{
		throw std::runtime_error("Blend state error 3");
	}
}

D3DHandler::~D3DHandler()
{	
	if (swapChain)
		swapChain->SetFullscreenState(false, NULL);

	if (alphaFewOverlappingBlendingState)
	{
		alphaFewOverlappingBlendingState->Release();
		alphaFewOverlappingBlendingState = nullptr;
	}

	if (alphaSeveralOverlappingBlendingState)
	{
		alphaSeveralOverlappingBlendingState->Release();
		alphaSeveralOverlappingBlendingState = nullptr;
	}

	if (alphaDisableBlendingState)
	{
		alphaDisableBlendingState->Release();
		alphaDisableBlendingState = nullptr;
	}

	if (depthDisabledStencilState)
	{
		depthDisabledStencilState->Release();
		depthDisabledStencilState = nullptr;
	}

	if (rasterState)
	{
		rasterState->Release();
		rasterState = nullptr;
	}

	if (depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = nullptr;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}
}

void D3DHandler::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(renderTargetView, color);

	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DHandler::EndScene()
{
	swapChain->Present(0, 0);
}


ID3D11Device* D3DHandler::GetDevice()
{
	return device;
}


ID3D11DeviceContext* D3DHandler::GetDeviceContext()
{
	return deviceContext;
}

void D3DHandler::DisableDepthStencil()
{
	deviceContext->OMSetDepthStencilState(depthDisabledStencilState, 1);
}

void D3DHandler::EnableDepthStencil()
{
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
}
void D3DHandler::EnableAlphaBlendingFewOverlapping()
{
	deviceContext->OMSetBlendState(alphaFewOverlappingBlendingState, NULL, 0xffffff);
}

void D3DHandler::EnableAlphaBlendingSeverlOverlapping()
{
	deviceContext->OMSetBlendState(alphaSeveralOverlappingBlendingState, NULL, 0xffffff);
}

void D3DHandler::DisableAlphaBlening()
{
	deviceContext->OMSetBlendState(alphaDisableBlendingState, NULL, 0xffffff);
}

void D3DHandler::GetProjectionMatrix(XMFLOAT4X4& projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
}


void D3DHandler::GetWorldMatrix(XMFLOAT4X4& worldMatrix)
{
	worldMatrix = this->worldMatrix;
}


void D3DHandler::GetOrthoMatrix(XMFLOAT4X4& orthoMatrix)
{
	orthoMatrix = this->orthoMatrix;
}
