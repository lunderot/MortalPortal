#include "PowerBar.h"

PowerBar::PowerBar(ID3D11Device* device)
{
	posColor.pos[0] = DirectX::XMFLOAT2(-0.7f, 1.0f);
	posColor.pos[1] = DirectX::XMFLOAT2(-0.7f, 0.9f);
	posColor.pos[2] = DirectX::XMFLOAT2(-0.7f, 1.0f);
	posColor.pos[3] = DirectX::XMFLOAT2(-0.7f, 0.9f);
	posColor.color.x = 0.0f;
	posColor.color.y = 0.0f;

	max = -0.5f;
	min = -0.7f;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(PosColor);
	bufferDesc.StructureByteStride = sizeof(PosColor);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &posColor.pos[0];
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}

	barSpeed = 0.00001f;
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

const DirectX::XMFLOAT2* PowerBar::GetPosition()
{
	return posColor.pos;
}

void PowerBar::SetMaxMinValue(DirectX::XMFLOAT2 value)
{
	this->max = value.x;
	this->min = value.y;
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


	if (posColor.pos[0].x > max)
	{
		barSpeed = barSpeed * - 1;
	} 
	if (posColor.pos[0].x < min)
	{
		barSpeed = barSpeed * -1;
	}

	posColor.pos[0].x += barSpeed;
	posColor.pos[1].x += barSpeed;
	//resource.pData = point;
	memcpy(resource.pData, &posColor, sizeof(PosColor));
	deviceContext->Unmap(vertexBuffer, 0);

	deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);

}
PowerBar::~PowerBar()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}
