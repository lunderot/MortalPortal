#include "PlayerWins.h"

PlayerWins::PlayerWins(Material* mat1, Material* mat2, ID3D11Device* device)
{
	this->material1 = mat1;
	this->material2 = mat2;

	buttonPoint points[4] =
	{
		DirectX::XMFLOAT2(-0.2f, -0.2f),
		DirectX::XMFLOAT2(0.0f, 1.0f),

		DirectX::XMFLOAT2(-0.2f, 0.2f),
		DirectX::XMFLOAT2(0.0f, 0.0f),

		DirectX::XMFLOAT2(0.2f, -0.2f),
		DirectX::XMFLOAT2(1.0f, 1.0f),

		DirectX::XMFLOAT2(0.2f, 0.2f),
		DirectX::XMFLOAT2(1.0f, 0.0f)

	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * 4;
	bufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT4);
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &points;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}
	player1Wins = false;

}


void PlayerWins::RenderText(ID3D11DeviceContext* deviceContext)
{
	unsigned int vertexSize = sizeof(float) * 4;
	unsigned int offset = 0;
	unsigned int vertexCount = 4;

	if (player1Wins == true)
		SRV = material1->GetTexture();
	else
		SRV = material2->GetTexture();
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	deviceContext->PSSetShaderResources(0, 1, &SRV);
	deviceContext->Draw(vertexCount, 0);
}

PlayerWins::~PlayerWins()
{
	if (vertexBuffer)
		vertexBuffer->Release();
}