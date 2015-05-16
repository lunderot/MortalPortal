#include "StartMenu.h"
#include <iostream>
StartMenu::StartMenu(ID3D11Device* device)
{
	check = 0;
	renderMenu = true;
	currentSelect = 0;
	scalingOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	rotationOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	ID3D11Buffer* buttonVertexBuffer = nullptr;
	buttonScale.button = true;
	buttonPoint points[4] =
	{
		DirectX::XMFLOAT2(-1.0f, -1.0f),
		DirectX::XMFLOAT2(0.0f, 1.0f),

		DirectX::XMFLOAT2(-1.0f, 1.0f),
		DirectX::XMFLOAT2(0.0f, 0.0f),

		DirectX::XMFLOAT2(1.0f, -1.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f),

		DirectX::XMFLOAT2(1.0f, 1.0f),
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

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &buttonVertexBuffer);
	buttonGeometry = new Geometry(buttonVertexBuffer, 4, nullptr);

	//Constant Buffer
	memset(&bufferDesc, 0, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ButtonScale);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = device->CreateBuffer(&bufferDesc, 0, &constantBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer in startMenu");
	}

}

void StartMenu::Update(bool up, bool down, bool enter)
{
	// 0 = start & quit = 1
	if (down == true && currentSelect >= 0 && currentSelect < buttons.size() - 1 && check == 0)
	{
		currentSelect++;
		check = 1;
		std::cout << "DOWN" << std::endl;
	}
	if (up == true && currentSelect > 0 && currentSelect < buttons.size() && check == 0)
	{
		check = 1;
		currentSelect--;
		std::cout << "UP" << std::endl;
	}
	if (enter == true && check == 0)
	{
		check = 1;
		renderMenu = buttons[currentSelect]->isClicked();
	}
	if (enter == false && up == false && down == false)
		check = 0;

}

void StartMenu::Render(ID3D11DeviceContext* deviceContext)
{
	ID3D11Buffer* vertexBuffer = buttonGeometry->GetVertexBuffer();
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		unsigned int vertexSize = sizeof(float) * 4;
		unsigned int offset = 0;
		unsigned int vertexCount = 4;
		if (currentSelect == i)
			scaling = DirectX::XMVectorSet(buttons[i]->scale.x + 0.1, buttons[i]->scale.y + 0.1, 1.0f, 1.0f);
		else
			scaling = DirectX::XMVectorSet(buttons[i]->scale.x, buttons[i]->scale.y, 1.0f, 1.0f);
		translation = DirectX::XMVectorSet(buttons[i]->position.x, buttons[i]->position.y, 1.0f, 1.0f);

		buttonScale.scale = DirectX::XMMatrixTranspose(DirectX::XMMatrixTransformation2D(scalingOrigin, 0.0f, scaling, rotationOrigin, 0.0f, translation));
		UpdateConstantBuffer(deviceContext, &buttonScale);
		deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		SRV = buttons[i]->material->GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &SRV);
		deviceContext->Draw(vertexCount, 0);
	}
}

Geometry* StartMenu::GetButtonGeometry()
{
	return buttonGeometry;
}

void StartMenu::UpdateConstantBuffer(ID3D11DeviceContext* deviceContext, ButtonScale* buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, buffer, sizeof(ButtonScale));
	deviceContext->Unmap(constantBuffer, 0);
}

void StartMenu::AddButton(Button* button)
{
	//battons = button;
	std::cout << button->position.x << button->position.y << std::endl;
	buttons.push_back(button);
}

StartMenu::~StartMenu()
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		delete buttons[i];
	}

	if (buttonVertexBuffer)
		buttonVertexBuffer->Release();
	delete buttonGeometry;
}