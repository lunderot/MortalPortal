#include "PowerBar.h"

PowerBar::PowerBar(ID3D11Device* device)
{
	posColor.pos[0] = DirectX::XMFLOAT2(-0.7f, 1.0f);
	posColor.pos[1] = DirectX::XMFLOAT2(-0.7f, 0.9f);
	posColor.pos[2] = DirectX::XMFLOAT2(-0.7f, 1.0f);
	posColor.pos[3] = DirectX::XMFLOAT2(-0.7f, 0.9f);
	posColor.color.x = 0.0f;
	posColor.color.y = 0.0f;

	maxMinValue.x = -0.5f;
	maxMinValue.y = -0.7f;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(PosColor);
	bufferDesc.StructureByteStride = sizeof(PosColor);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &posColor.pos[0];
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}

	barSpeed = 0.001f;
}
void PowerBar::SetBarSpeed(float speed)
{
	this->barSpeed = speed;
}
void PowerBar::SetPosition(DirectX::XMFLOAT2 point[4])
{
	this->posColor.pos[0] = point[0];
	this->posColor.pos[1] = point[1];
	this->posColor.pos[2] = point[2];
	this->posColor.pos[3] = point[3];
}

void PowerBar::SetColor(DirectX::XMFLOAT2 color)
{
	this->posColor.color.x = color.x;
	this->posColor.color.y = color.y;
}

void PowerBar::SetMaxMinValue(DirectX::XMFLOAT2 value)
{
	this->maxMinValue.x = value.x;
	this->maxMinValue.y = value.y;
}

const float PowerBar::GetBarSpeed()
{
	return barSpeed;
}

const DirectX::XMFLOAT2* PowerBar::GetPosition()
{
	return posColor.pos;
}

const DirectX::XMFLOAT2 PowerBar::GetMaxMinValue()
{
	return maxMinValue;
}


ID3D11Buffer* PowerBar::GetVertexBuffer()
{
	return vertexBuffer;
}

void PowerBar::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	unsigned int vertexSize = sizeof(DirectX::XMFLOAT2);
	unsigned int offset = 0;
	unsigned int vertexCount = 4;
	ID3D11Buffer* vb = vertexBuffer;

	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT result;
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);


	memcpy(resource.pData, &posColor, sizeof(PosColor));
	deviceContext->Unmap(vertexBuffer, 0);

	deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);

}

void PowerBar::Update(float deltaTime)
{
	if (posColor.pos[0].x > maxMinValue.x)
	{
		barSpeed = barSpeed * -1;
	}
	if (posColor.pos[0].x < maxMinValue.y)
	{
		barSpeed = barSpeed * -1;
	}

	posColor.pos[0].x += barSpeed;
	posColor.pos[1].x += barSpeed;
}
PowerBar::~PowerBar()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}
