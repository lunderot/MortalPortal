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
	for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end();)
	{
		if (!(*i)->GetAlive())
		{
			delete (*i);
			i = entities.erase(i);
		}
		else
		{
			(*i)->Update(deltaTime);
			++i;
		}
		
	}
	for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		for (std::vector<Entity*>::iterator j = entities.begin(); j != entities.end(); ++j)
		{
			if ((*i) != (*j))
			{
				Collision* collision1 = (*i)->GetGeometry()->GetCollision();
				Collision* collision2 = (*j)->GetGeometry()->GetCollision();

				
				XMFLOAT3 position = (*i)->GetPosition();
				XMFLOAT3 rotation = (*i)->GetRotation();
				XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

				XMMATRIX model1 = XMMatrixRotationQuaternion(rotationQuat);
				model1 = XMMatrixMultiply(model1, XMMatrixTranslationFromVector(XMLoadFloat3(&position)));


				position = (*j)->GetPosition();
				rotation = (*j)->GetRotation();
				rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

				XMMATRIX model2 = XMMatrixRotationQuaternion(rotationQuat);
				XMFLOAT4X4 worldMatrix2;
				model2 = XMMatrixMultiply(model2, XMMatrixTranslationFromVector(XMLoadFloat3(&position)));

				bool collision = false;
				for (std::vector<CollisionSphere>::iterator k = collision1->spheres.begin(); k != collision1->spheres.end() && !collision; ++k)
				{
					for (std::vector<CollisionSphere>::iterator l = collision2->spheres.begin(); l != collision2->spheres.end() && !collision; ++l)
					{
						if (IsSpheresColliding((*k), (*l), model1, model2))
						{
							HandleCollision((*i), (*j));
							collision = true;
						}
					}
				}
			}
		}
		
	}
}

void EntityHandler::Render(ID3D11DeviceContext* deviceContext)
{
	for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		Shader* currentShader = (*i)->GetShader();
		Geometry* geometry = (*i)->GetGeometry();
		Material* material = (*i)->GetMaterial();

		if (currentShader == nullptr)
		{
			throw std::runtime_error("Shader pointer is nullptr");
		}
		if (geometry == nullptr)
		{
			throw std::runtime_error("Geometry pointer is nullptr");
		}
		if (material == nullptr)
		{
			throw std::runtime_error("Material pointer is nullptr");
		}

		//Update per model constant buffer
		XMFLOAT3 position = (*i)->GetPosition();
		XMFLOAT3 rotation = (*i)->GetRotation();
		XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

		ConstantBufferPerModel data;
		XMMATRIX model = XMMatrixRotationQuaternion(rotationQuat);
		model = XMMatrixMultiply(model, XMMatrixTranslation(position.x, position.y, position.z));
		
		XMStoreFloat4x4(&data.worldMatrix, XMMatrixTranspose(model));
		currentShader->UpdateConstantBufferPerModel(deviceContext, &data);

		//Use the current entity shader
		currentShader->Use(deviceContext);

		//Draw the mesh
		unsigned int vertexSize = sizeof(VertexPositionTexCoordNormalBinormalTangent);
		unsigned int vertexCount = (*i)->GetGeometry()->GetVertexCount();
		unsigned int offset = 0;
		ID3D11Buffer* vb = (*i)->GetGeometry()->GetVertexBuffer();

		deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//TODO: Use material to set texture
		ID3D11ShaderResourceView* texure = material->GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &texure);

		deviceContext->Draw(vertexCount, 0);
	}
}

void EntityHandler::Add(Entity* entity)
{
	entities.push_back(entity);
}

void EntityHandler::HandleCollision(Entity* entity1, Entity* entity2)
{
	if (entity1->GetMaterial() == entity2->GetMaterial())
	{
		if (dynamic_cast<Player*>(entity1))
		{
			if (dynamic_cast<MapItem*>(entity2))
			{
				entity2->SetAlive(false);
				dynamic_cast<Player*>(entity1)->AddPower();
			}
		}
	}
	else if (entity1->GetMaterial() != entity2->GetMaterial())
		if (dynamic_cast<Player*>(entity1))
		{
			if (dynamic_cast<MapItem*>(entity2))
			{
				entity2->SetAlive(false);
				dynamic_cast<Player*>(entity1)->RemovePower();
			}
		}
}
