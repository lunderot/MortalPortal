#include "ComboBar.h"

ComboBar::ComboBar(ID3D11Device* device, Material* materialCombo)
{
	Material* materialCombo_Array[2] = { materialCombo };
	this->setMaterial(materialCombo_Array);
	changeCombo = 0;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(ComboPoints);
	bufferDesc.StructureByteStride = sizeof(ComboPoints);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &comboPoints;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}
}

void ComboBar::setMaterial(Material* materialCombo[2])
{
	this->materialUsing[0] = materialCombo[0];
	this->materialUsing[1] = materialCombo[1];
}

void ComboBar::SetPosition(DirectX::XMFLOAT2 point[4])
{
	this->comboPoints.pos[0] = point[0];
	this->comboPoints.pos[1] = point[1];
	this->comboPoints.pos[2] = point[2];
	this->comboPoints.pos[3] = point[3];
}

void ComboBar::SetUV(DirectX::XMFLOAT2 UV[4])
{
	this->comboPoints.uv[0] = UV[0];
	this->comboPoints.uv[1] = UV[1];
	this->comboPoints.uv[2] = UV[2];
	this->comboPoints.uv[3] = UV[3];
}

const DirectX::XMFLOAT2* ComboBar::GetPosition()
{
	return comboPoints.pos;
}

ID3D11Buffer* ComboBar::GetVertexBuffer()
{
	return vertexBuffer;
}

void ComboBar::AddCombo()
{
	changeCombo++;
}

void ComboBar::RemoveCombo()
{
	changeCombo = 0;
}

void ComboBar::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	unsigned int vertexSize = sizeof(DirectX::XMFLOAT4);
	UINT test = sizeof(DirectX::XMFLOAT2);
	UINT32 offset = 0;
	unsigned int vertexCount = 4;

	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT result;
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);


	memcpy(resource.pData, &comboPoints, sizeof(ComboPoints));
	deviceContext->Unmap(vertexBuffer, 0);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &test, &offset);
	if (changeCombo <= 0)
	{
		SRV = materialUsing[0]->GetTexture();
	}
	else if (changeCombo > 0)
	{
		SRV = materialUsing[1]->GetTexture();
	}
	deviceContext->PSSetShaderResources(0, 1, &SRV);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);


}

void ComboBar::Update(float deltaTime)
{

}

ComboBar::~ComboBar()
{
	if (SRV)
	{
		SRV->Release();
	}

	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}
