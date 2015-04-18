#include "Application.h"

using namespace DirectX;

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
			: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	float screenFar = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenFar, screenNear);

	testShader = new Shader(d3dHandler->GetDevice(), screenWidth, screenHeight, screenNear, screenFar);

	try
	{
		input = new ControllerInput();
		std::cout << "Using controller input" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Using keyboard input" << std::endl;
		input = new KeyboardInput();
	}

	testImporter.importFile("assets/test.bin");
	

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	testShader->CreateMandatoryShaders(d3dHandler->GetDevice(), L"assets/shaders/vs.hlsl", L"assets/shaders/ps.hlsl", inputDesc, ARRAYSIZE(inputDesc));

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = testImporter.getMeshVertexCount(0) * sizeof(VertexPositionTexCoordNormalBinormalTangent);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = testImporter.getMesh(0);
	HRESULT hr = d3dHandler->GetDevice()->CreateBuffer(&bufferDesc, &data, &testVertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}
}

Application::~Application()
{
	delete d3dHandler;
	delete testShader;
	delete input;

	if (testVertexBuffer)
		testVertexBuffer->Release();
}

bool Application::Update(float deltaTime)
{
	XMFLOAT2 controllerDir = input->GetDirection();

	ConstantBufferPerModel data;
	XMMATRIX model = XMMatrixTranslation(controllerDir.x * 10, controllerDir.y * 10, input->GetButtonState()*10);
	XMStoreFloat4x4(&data.worldMatrix, XMMatrixTranspose(model));

	testShader->UpdateConstantBufferPerModel(d3dHandler->GetDeviceContext(), &data);

	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	testShader->Use(d3dHandler->GetDeviceContext());

	unsigned int vertexSize = sizeof(VertexPositionTexCoordNormalBinormalTangent);
	unsigned int vertexCount = testImporter.getMeshVertexCount(0);
	unsigned int offset = 0;

	d3dHandler->GetDeviceContext()->IASetVertexBuffers(0, 1, &testVertexBuffer,  &vertexSize, &offset);
	d3dHandler->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dHandler->GetDeviceContext()->Draw(vertexCount, 0);

	d3dHandler->EndScene();
}