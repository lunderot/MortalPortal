#include "Background.h"

using namespace DirectX;

Background::Background(ID3D11Device* device) : Entity(nullptr, nullptr, nullptr)
{
	ID3D11Buffer* vertexBuffer = nullptr;

	struct vertex
	{
		float x, u, z;
		float r, g, b;
		//float u, v;
	}
	plane[12] =
	{
		// TriangleList


		-1.0f, -1.0f, 100.0f, // 1
		1.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 100.0f, // 0
		0.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 100.0f, // 2
		0.0f, 0.0f, 1.0f,



		0.0f, 1.0f, 100.0f, // 3
		1.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 100.0f, // 5
		1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 100.0f, // 4
		1.0f, 0.5f, 0.0f,



		0.0f, -1.0f, 0.0f, // 6
		0.5f, 0.3f, 0.0f,

		0.0f, 1.0f, 0.0f, // 7
		1.0f, 1.0f, 0.3f,

		1.0f, 1.0f, 0.0f, // 8
		1.0f, 1.0f, 1.0f,


		0.0f, -1.0f, 0.0f, // 10
		0.0f, 0.0f, 0.0f,

		1.0, 1.0f, 0.0f, // 9
		0.8f, 0.8f, 0.1f,

		1.0f, -1.0f, 0.0f, // 11
		0.9f, 0.1f, 0.5f

	};

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof(plane);
	bufferDesc.ByteWidth = ARRAYSIZE(plane) * sizeof(vertex);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = plane;
	//data.pSysMem = &plane;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	geometry = new Geometry(vertexBuffer, 12);


}


void Background::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	ID3D11Buffer* vertexBuffer = geometry->GetVertexBuffer();

	UINT32 vertexSize = sizeof(float)* 6;
	UINT32 offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(geometry->GetVertexCount(), 0);
}


Background::~Background()
{
	delete geometry;
};


//1280, 720,



// -1,-1 ---------------- 1,-1
//       |     0,0      |
// -1,1  ---------------- 1,1