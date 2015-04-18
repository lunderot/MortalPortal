#include "EntityHandler.h"

using namespace DirectX;

EntityHandler::EntityHandler()
{
	
}


EntityHandler::~EntityHandler()
{
	for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		delete *i;
	}
}

void EntityHandler::Update(float deltaTime)
{
	for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		(*i)->Update(deltaTime);
	}
}

void EntityHandler::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		//Update per model constant buffer
		XMFLOAT2 playerPosition = (*i)->GetPosition();

		ConstantBufferPerModel data;
		XMMATRIX model = XMMatrixTranslation(playerPosition.x, playerPosition.y, 0);
		XMStoreFloat4x4(&data.worldMatrix, XMMatrixTranspose(model));
		shader->UpdateConstantBufferPerModel(deviceContext, &data);

		//Draw the mesh
		unsigned int vertexSize = sizeof(VertexPositionTexCoordNormalBinormalTangent);
		unsigned int vertexCount = (*i)->GetVertexCount();
		unsigned int offset = 0;
		ID3D11Buffer* vb = (*i)->GetVertexBuffer();

		deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		deviceContext->Draw(vertexCount, 0);
	}
}

void EntityHandler::Add(Entity* entity)
{
	entities.push_back(entity);
}
