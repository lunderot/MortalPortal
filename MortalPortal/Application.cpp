#include "Application.h"

Application::Application(bool fullscreen, bool showCursor, int screenWidth, int screenHeight, LPCWSTR windowTitle)
			: System(fullscreen, showCursor, screenWidth, screenHeight, windowTitle)
{
	float screenDepth = 1000.0f;
	float screenNear = 0.1f;
	d3dHandler = new D3DHandler(screenWidth, screenHeight, hwnd, fullscreen, screenDepth, screenNear);

	testShader = new Shader(d3dHandler->GetDevice(), screenWidth, screenHeight);

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
	

	/*D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	*/

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	testShader->CreateMandatoryShaders(d3dHandler->GetDevice(), L"assets/shaders/vs.hlsl", L"assets/shaders/ps.hlsl", inputDesc, ARRAYSIZE(inputDesc));

	struct tv
	{
		float x, y, z;
		float u, v;
	}
	triangleVertices[6] =
	{
		//First triangle
		-0.5f, 0.5f, 0.0f,
		0.0f, 0.0f,

		0.5f, -0.5f, 0.0f,
		1.0f, 1.0f,

		-0.5f, -0.5f, 0.0f,
		0.0f, 1.0f,

		//Second triangle
		-0.5f, 0.5f, 0.0f,
		0.0f, 0.0f,

		0.5f, 0.5f, 0.0f,
		1.0f, 0.0f,

		0.5f, -0.5f, 0.0f,
		1.0f, 1.0f
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
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
	return false;
}

void Application::Render()
{
	d3dHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	testShader->Use(d3dHandler->GetDeviceContext());

	unsigned int vertexSize = sizeof(float) * 5;
	unsigned int vertexCount = 6;
	unsigned int offset = 0;

	d3dHandler->GetDeviceContext()->IASetVertexBuffers(0, 1, &testVertexBuffer,  &vertexSize, &offset);
	d3dHandler->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dHandler->GetDeviceContext()->Draw(6, 0);

	d3dHandler->EndScene();
}