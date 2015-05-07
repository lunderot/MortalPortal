#include "PowerUpDisplay.h"

PowerUpDisplayText::PowerUpDisplayText(ID3D11Device* device, Material* materialCombo)
{
	Material* materialCombo_Array[2] = { materialCombo };
	this->setMaterial(materialCombo_Array);
	comboText = false;
	comboNr = 1;
	SRV = nullptr;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(PowerUpDisplayTextPoints);
	bufferDesc.StructureByteStride = sizeof(PowerUpDisplayTextPoints);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &powerUpDTPoints;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}
}

void PowerUpDisplayText::setMaterial(Material* materialCombo[2])
{
	/*for (unsigned int i = 0; i < 4; i++)
	{
	this->materialUsing[i] = materialCombo[i];
	}*/
	this->materialUsing[0] = materialCombo[0];
	this->materialUsing[1] = materialCombo[1];
	//this->materialUsing[4] = materialCombo[4];
	//this->materialUsing[5] = materialCombo[5];
	//this->materialUsing[6] = materialCombo[6];
	//this->materialUsing[7] = materialCombo[7];
	//this->materialUsing[8] = materialCombo[8];
	//this->materialUsing[9] = materialCombo[9];
	//this->materialUsing[10] = materialCombo[10];
}

void PowerUpDisplayText::SetPosition(DirectX::XMFLOAT2 point[4])
{
	this->powerUpDTPoints.pos[0] = point[0];
	this->powerUpDTPoints.pos[1] = point[1];
	this->powerUpDTPoints.pos[2] = point[2];
	this->powerUpDTPoints.pos[3] = point[3];
}

void PowerUpDisplayText::SetUV(DirectX::XMFLOAT2 UV[4])
{
	this->powerUpDTPoints.uv[0] = UV[0];
	this->powerUpDTPoints.uv[1] = UV[1];
	this->powerUpDTPoints.uv[2] = UV[2];
	this->powerUpDTPoints.uv[3] = UV[3];
}

const DirectX::XMFLOAT2* PowerUpDisplayText::GetPosition()
{
	return powerUpDTPoints.pos;
}

ID3D11Buffer* PowerUpDisplayText::GetVertexBuffer()
{
	return vertexBuffer;
}

void PowerUpDisplayText::AddTextDisplay()
{
	comboNr = 0;
}

void PowerUpDisplayText::RemoveTextDisplay()
{
	comboNr = 1;
}

void PowerUpDisplayText::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	unsigned int vertexSize = sizeof(DirectX::XMFLOAT4);
	UINT test = sizeof(DirectX::XMFLOAT2);
	UINT32 offset = 0;
	unsigned int vertexCount = 4;

	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT result;
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);


	memcpy(resource.pData, &powerUpDTPoints, sizeof(PowerUpDisplayTextPoints));
	deviceContext->Unmap(vertexBuffer, 0);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &test, &offset);
	//if (comboText == true)
	//{
	//	// "COMBO" - text
	//	SRV = materialUsing[0]->GetTexture();
	//}
	//else if (comboText == false)
	//{
	//	// comboNr | 0 = combo text
	//	// comboNr | 1 - 10 = siffror : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	SRV = materialUsing[comboNr]->GetTexture();
	/*}*/

	deviceContext->PSSetShaderResources(0, 1, &SRV);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(vertexCount, 0);


}

void PowerUpDisplayText::Update(float deltaTime)
{

}

PowerUpDisplayText::~PowerUpDisplayText()
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
