#include "GameOver.h"

GameOver::GameOver(Points points, ID3D11Device* device)
{
	this->points = points;
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(Points);
	bufferDesc.StructureByteStride = sizeof(Points);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &points;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}

	
}


void GameOver::RenderText(ID3D11DeviceContext* deviceContext)
{
	unsigned int vertexSize = sizeof(DirectX::XMFLOAT4);
	unsigned int offset = 0;
	unsigned int vertexCount = 4;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);
}

GameOver::~GameOver()
{
	if (vertexBuffer)
		vertexBuffer->Release();
}