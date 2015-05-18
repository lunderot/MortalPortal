#include "EntityHandler.h"

using namespace DirectX;

EntityHandler::EntityHandler()
{

}


EntityHandler::~EntityHandler()
{
	for (std::map<Shader*, std::vector<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::vector<Entity*>::iterator i = ent->second.begin(); i != ent->second.end(); ++i)
		{
			delete *i;
		}
	}
}

void EntityHandler::Update(float deltaTime, AudioMaster &aMaster)
{
	std::vector<Player*> player;
	for (std::map<Shader*, std::vector<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::vector<Entity*>::iterator i = ent->second.begin(); i != ent->second.end();++i)
		{
			Player* tempPlayer = dynamic_cast<Player*>(*i);
			if (tempPlayer)
			{
				player.push_back(tempPlayer);
			}
		}
	}
	for (std::map<Shader*, std::vector<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::vector<Entity*>::iterator i = ent->second.begin(); i != ent->second.end();)
		{
			if (!(*i)->GetAlive())
			{
				MapItem* item = dynamic_cast<MapItem*>(*i);
				if (item)
				{
					//If item is a crystal and outside the screen
					if (item->type == MapItem::objectType::Crystal && item->GetPosition().x < -30)
					{
						//Check players who has the color of the crystal
						for (int i = 0; i < 2; i++)
						{
							//Remove combo if it was the player's color
							if (player[i]->HasColor(item->GetColor()))
							{
								player[i]->RemoveComboText();
								player[i]->RemoveCombo();
							}
						}
					}
				}
				delete (*i);
				i = ent->second.erase(i);
			}
			else
			{
				(*i)->Update(deltaTime);
				++i;
			}
		}
	}
	for (std::map<Shader*, std::vector<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::vector<Player*>::iterator i = player.begin(); i != player.end(); ++i)
		{
			for (std::vector<Entity*>::iterator j = ent->second.begin(); j != ent->second.end(); ++j)
			{
				if ((*i) != (*j))
				{
					Collision* collision1 = (*i)->GetGeometry()->GetCollision();
					Collision* collision2 = (*j)->GetGeometry()->GetCollision();		

					XMFLOAT3 position = (*i)->GetPosition();
					XMFLOAT3 rotation = (*i)->GetRotation();
					XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
					//H�r - Scaling
					XMFLOAT3 scaling = (*i)->GetScale();
					XMMATRIX scaleMatrix = XMMatrixScaling(scaling.x, scaling.y, scaling.z);

					XMMATRIX model1 = XMMatrixRotationQuaternion(rotationQuat);
					model1 = XMMatrixMultiply(model1, XMMatrixTranslationFromVector(XMLoadFloat3(&position)));
					model1 = scaleMatrix * model1;
					
					position = (*j)->GetPosition();
					rotation = (*j)->GetRotation();
					rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

					scaling = (*j)->GetScale();
					scaleMatrix = XMMatrixScaling(scaling.x, scaling.y, scaling.z);

					XMMATRIX model2 = XMMatrixRotationQuaternion(rotationQuat);
					XMFLOAT4X4 worldMatrix2;
					model2 = XMMatrixMultiply(model2, XMMatrixTranslationFromVector(XMLoadFloat3(&position)));

					model2 = scaleMatrix * model2;

					for (std::vector<CollisionSphere>::iterator k = collision1->spheres.begin(); k != collision1->spheres.end(); ++k)
					{
						for (std::vector<CollisionSphere>::iterator l = collision2->spheres.begin(); l != collision2->spheres.end(); ++l)
						{
							if (IsSpheresColliding((*k), (*l), model1, model2))
							{
								HandleCollision((*i), (*j), (*k).name, aMaster);
							}
						}
					}
				}
			}
		}
	}
}

void EntityHandler::Render(ID3D11DeviceContext* deviceContext, D3DHandler* d3dHandler)
{
	for (std::map<Shader*, std::vector<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		//Use the current entity shader
		Shader* currentShader = ent->first;
		currentShader->Use(deviceContext);

		for (std::vector<Entity*>::iterator i = ent->second.begin(); i != ent->second.end(); ++i)
		{
			if ((*i)->GetVisible())
			{
				if (dynamic_cast<PowerupIndicator*>(*i))
				{
					d3dHandler->EnableAlphaBlendingSeverlOverlapping();
				}

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
				//H�r - Scaling
				XMFLOAT3 scaling = (*i)->GetScale();
				XMMATRIX scaleMatrix = XMMatrixScaling(scaling.x, scaling.y, scaling.z);

				model = XMMatrixMultiply(model, XMMatrixTranslation(position.x, position.y, position.z));

				model = scaleMatrix * model;

				XMStoreFloat4x4(&data.worldMatrix, XMMatrixTranspose(model));
				currentShader->UpdateConstantBufferPerModel(deviceContext, &data);



				//Draw the mesh
				unsigned int vertexSize = sizeof(VertexPositionTexCoordNormalBinormalTangent);
				unsigned int vertexCount = (*i)->GetGeometry()->GetVertexCount();
				unsigned int offset = 0;
				ID3D11Buffer* vb = (*i)->GetGeometry()->GetVertexBuffer();

				deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
				deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				ID3D11ShaderResourceView* texture = material->GetTexture();
				deviceContext->PSSetShaderResources(0, 1, &texture);

				ID3D11ShaderResourceView* normal_map = material->GetNormalMap();
				deviceContext->PSSetShaderResources(1, 1, &normal_map);

				ID3D11ShaderResourceView* specular_map = material->GetSpecularMap();
				deviceContext->PSSetShaderResources(2, 1, &specular_map);

				deviceContext->PSSetConstantBuffers(1, 1, &material->pointerToBufferM);

				deviceContext->Draw(vertexCount, 0);

				if (dynamic_cast<PowerupIndicator*> (*i))
				{
					d3dHandler->EnableAlphaBlendingFewOverlapping();
				}
			}
		}
	}
}

void EntityHandler::Add(Entity* entity)
{
	entities[entity->GetShader()].push_back(entity);
}

void EntityHandler::HandleCollision(Player* player, Entity* entity2, std::string name, AudioMaster &aMaster)
{
	MapItem* item = dynamic_cast<MapItem*>(entity2);
	bool isPortal = name == "nurbsSphere1";

	if (item)
	{
		switch (item->type)
		{
			case MapItem::objectType::Comet:
			{
				if (player->getImmortalPortal() == true)
				{
					player->AddCombo(false);
					player->AddComboText();
					player->AddPower(player->comboBar->GetComboCount());
					player->renderParticles = true;
					player->doubleUp = true;
					aMaster.playSample("boing");
				}
				else
				{
					player->RemovePower();
					player->RemoveCombo();
					player->RemoveComboText();
					player->AddScore(-20);
				}
				break;
			}	
			case MapItem::objectType::PowerUp:
			{
				if (isPortal)
				{
					unsigned int rnd = rand() % 5;

					if (rnd == 0) // Slow Down Acceleration
					{
						player->setSlowDownAcceleration(5.0f);
					}

					if (rnd == 1) // Immortal Portal
					{
						player->setImmortalPortal(5.0f);
					}

					if (rnd == 2) // Combo Bonus
					{
						player->setBonusCombo(5.0f);
					}

					if (rnd == 3) // Crystal Frenzy
					{
						player->setCrystalFrenzy(5.0f);
					}

					if (rnd == 4) // Inverse Control
					{
						player->setInvertControl(5.0f);
					}
				}

				break;
			}
			case MapItem::objectType::Crystal:
			{
				if (player->GetColor() == item->GetColor() && isPortal)
				{
					player->AddCombo(false);
					player->AddComboText();
					player->AddPower(player->comboBar->GetComboCount());
					player->renderParticles = true;
					player->doubleUp = true;
					aMaster.playSample("boing");
					player->AddScore(100);
				}
				else
				{
					if (player->getImmortalPortal() == true)
					{
						player->AddCombo(false);
						player->AddComboText();
						player->AddPower(player->comboBar->GetComboCount());
						player->renderParticles = true;
						player->doubleUp = true;
						aMaster.playSample("boing");
					}
					else
					{
						player->RemovePower();
						player->RemoveCombo();
						player->RemoveComboText();
						player->AddScore(-20);
					}
				}
				break;
			}
			default:
				break;
		}
		item->SetAlive(false);
	}
}

void EntityHandler::KillAllMapItems()
{
	for (std::map<Shader*, std::vector<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::vector<Entity*>::iterator i = ent->second.begin(); i != ent->second.end(); ++i)
		{
		
			if (dynamic_cast<MapItem*> (*i))
				(*i)->SetAlive(false);
		}
	}
}