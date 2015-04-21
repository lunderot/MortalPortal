#include "PowerBar.h"

PowerBar::PowerBar(ID3D11Device* device)
{
	point[0] = DirectX::XMFLOAT2(-0.5f, 1.0f);
	point[1] = DirectX::XMFLOAT2(-0.5f, 0.8f);
	point[2] = DirectX::XMFLOAT2(-1.0f, 1.0f);
	point[3] = DirectX::XMFLOAT2(-1.0f, 0.8f);

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT2) * 4;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &point[0];
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}
}

void PowerBar::setPosition(DirectX::XMFLOAT2 point[4])
{
	this->point[0] = point[0];
	this->point[1] = point[1];
	this->point[2] = point[2];
	this->point[3] = point[3];
}

const DirectX::XMFLOAT2* PowerBar::getPosition()
{
	return point;
}


ID3D11Buffer* PowerBar::getVertexBuffer()
{
	return vertexBuffer;
}

void PowerBar::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	unsigned int vertexSize = sizeof(DirectX::XMFLOAT2);
	unsigned int offset = 0;
	unsigned int vertexCount = 4;
	ID3D11Buffer* vb = getVertexBuffer();

	deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);
}
PowerBar::~PowerBar()
{

}