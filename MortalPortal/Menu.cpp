#include "Menu.h"
#include <iostream>
Menu::Menu()
{

}

void Menu::Update(float deltaTime)
{

}

void Menu::Render(ID3D11DeviceContext* deviceContext)
{
	for (unsigned int i = 0; i < 0; i++)
	{
		unsigned int vertexSize = sizeof(DirectX::XMFLOAT4);
		unsigned int offset = 0;
		unsigned int vertexCount = 4;
		ID3D11Buffer* vertexBuffer = buttons[i]->geometry->GetVertexBuffer();

		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		deviceContext->Draw(vertexCount, 0);
	}
}

void Menu::AddButton(Button* button)
{
	//battons = button;
	std::cout << button->position.x << button->position.y << std::endl;
	//buttons.push_back(button);
}

Menu::~Menu()
{

}