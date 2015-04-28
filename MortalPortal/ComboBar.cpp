#include "ComboBar.h"

ComboBar::ComboBar(ID3D11Device* device, Material* materialCombo)
{
	this->setMaterial(materialCombo);

	posColor.pos[0] = DirectX::XMFLOAT2(0.15f, 1.0f);
	posColor.pos[1] = DirectX::XMFLOAT2(0.15f, 0.8f);
	posColor.pos[2] = DirectX::XMFLOAT2(0.0f, 1.0f);
	posColor.pos[3] = DirectX::XMFLOAT2(0.0f, 0.8f);
	posColor.color.x = 0.0f;
	posColor.color.y = 0.0f;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(PosColorr);
	bufferDesc.StructureByteStride = sizeof(PosColorr);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &posColor.pos[0];
	HRESULT hr = device->CreateBuffer(&bufferDesc, 0, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}
}

void ComboBar::setMaterial(Material* materialCombo)
{
	this->materialUsing = materialCombo;
}

void ComboBar::SetPosition(DirectX::XMFLOAT2 point[4])
{
	this->posColor.pos[0] = point[0];
	this->posColor.pos[1] = point[1];
	this->posColor.pos[2] = point[2];
	this->posColor.pos[3] = point[3];
}

void ComboBar::SetColor(DirectX::XMFLOAT2 color)
{
	this->posColor.color.x = color.x;
	this->posColor.color.y = color.y;
}

const DirectX::XMFLOAT2* ComboBar::GetPosition()
{
	return posColor.pos;
}

ID3D11Buffer* ComboBar::GetVertexBuffer()
{
	return vertexBuffer;
}

void ComboBar::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	unsigned int vertexSize = sizeof(DirectX::XMFLOAT2);
	unsigned int offset = 0;
	unsigned int vertexCount = 4;

	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT result;
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);


	memcpy(resource.pData, &posColor, sizeof(PosColorr));
	deviceContext->Unmap(vertexBuffer, 0);

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);

}

void ComboBar::Update(float deltaTime)
{

}

ComboBar::~ComboBar()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}
