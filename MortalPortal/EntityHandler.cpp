#include "EntityHandler.h"

using namespace DirectX;

EntityHandler::EntityHandler()
{

}


EntityHandler::~EntityHandler()
{
	for (std::map<Shader*, std::list<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::list<Entity*>::iterator i = ent->second.begin(); i != ent->second.end();)
		{
			std::list<Entity*>::iterator tmp = i;
			i++;
			Entity* forDelete = *tmp;
			ent->second.erase(tmp);
			delete forDelete;		
		}
	}
}

void EntityHandler::Update(float deltaTime, AudioMaster &aMaster)
{
	std::vector<Player*> player;
	for (std::map<Shader*, std::list<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::list<Entity*>::iterator i = ent->second.begin(); i != ent->second.end(); ++i)
		{
			Player* tempPlayer = dynamic_cast<Player*>(*i);
			if (tempPlayer)
			{
				player.push_back(tempPlayer);
			}
		}
	}
	for (std::map<Shader*, std::list<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		int j = 0;
		for (std::list<Entity*>::iterator i = ent->second.begin(); i != ent->second.end();)
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
								player[i]->RemoveCombo();
							}
						}
					}
				}
				std::list<Entity*>::iterator tmp = ent->second.begin();
				advance(tmp, j);
				
				Entity* forDelete = *tmp;
				ent->second.erase(tmp);
				delete forDelete;

				i = ent->second.begin();
				j = -1;
			}
			else
			{
				(*i)->Update(deltaTime);
				++i;
			}
			j++;
		}
	}
	for (std::map<Shader*, std::list<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::vector<Player*>::iterator i = player.begin(); i != player.end(); ++i)
		{
			for (std::list<Entity*>::iterator j = ent->second.begin(); j != ent->second.end(); ++j)
			{
				if ((*i) != (*j))
				{
					Collision* collision1 = (*i)->GetGeometry()->GetCollision();
					Collision* collision2 = (*j)->GetGeometry()->GetCollision();		

					XMFLOAT3 position = (*i)->GetPosition();
					XMFLOAT3 rotation = (*i)->GetRotation();
					XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
					//Här - Scaling
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
	for (std::vector<Shader*>::iterator shader = shaderOrder.begin(); shader != shaderOrder.end(); ++shader)
	{
		//Use the current entity shader
		Shader* currentShader = *shader;
		currentShader->Use(deviceContext);

		if (dynamic_cast<OrthoHudShader*>(currentShader))
		{
			d3dHandler->EnableAlphaBlendingSeverlOverlapping();
		}
		else
		{
			d3dHandler->EnableAlphaBlendingFewOverlapping();
		}

		std::list<Entity*>& ent = entities[*shader];
		for (std::list<Entity*>::iterator i = ent.begin(); i != ent.end(); ++i)
		{
			if ((*i)->GetVisible())
			{
				
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
				//Här - Scaling
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
			}
		}
	}
}

void EntityHandler::Add(Entity* entity)
{
	if (std::find(shaderOrder.begin(), shaderOrder.end(), entity->GetShader()) == shaderOrder.end())
	{
		shaderOrder.push_back(entity->GetShader());
	}

	for (std::list<Entity*>::iterator entityList = entities[entity->GetShader()].begin(); entityList != entities[entity->GetShader()].end(); entityList++)
	{
		if (entity->position.z > (*entityList)->position.z)
		{
			entities[entity->GetShader()].insert(entityList, entity);
			return;
		}
	}
	entities[entity->GetShader()].push_back(entity);
}

void EntityHandler::HandleCollision(Player* player, Entity* entity2, std::string name, AudioMaster &aMaster)
{
	MapItem* item = dynamic_cast<MapItem*>(entity2);

	bool isPortal = name == "Portal";

	if (item)
	{
		switch (item->type)
		{
			case MapItem::objectType::Comet:
			{
				if (player->GetImmortalPortal() == true)
				{
					player->AddCombo(false);
					player->renderParticles = true;
					player->doubleUp = true;
					aMaster.playSample("boing");
				}
				else
				{
					//XINPUT_VIBRATION vibrationValue{ 65535, 65535 };
					//player->setVibrationOnController(&vibrationValue, 0.1f);
					player->RemoveBonus();
					player->RemovePower(0.02f);
					player->RemoveCombo();
					player->AddScore(-20);
					aMaster.playSample("Punch");
				}
				break;
			}	
			case MapItem::objectType::PowerUp:
			{
				if (isPortal)
				{
					unsigned int rnd = rand() % 4;

					if (rnd == 0) // Slow Down Acceleration
					{
						player->setSlowDownAcceleration(5.0f);
						aMaster.playSample("Freeze");
					}

					if (rnd == 1) // Immortal Portal
					{
						player->setImmortalPortal(5.0f);
						aMaster.playSample("Immortal");
					}

					if (rnd == 2) // Crystal Frenzy
					{
						player->setCrystalFrenzy(5.0f);
						aMaster.playSample("CrystalFrenzy");
					}

					if (rnd == 3) // Inverse Control
					{
						player->SetInvertControl(5.0f);
						aMaster.playSample("Invert");
					}

					//if (rnd == 2) // Combo Bonus
					//{
					//	player->setBonusCombo(5.0f);
					//}
				}

				break;
			}
			case MapItem::objectType::Crystal:
			{
				
				if (player->GetColor() == item->GetColor())
				{
					if (isPortal)
					{
						player->AddCombo(1);
						player->AddBonus(1);
						player->AddPower(player->GetCombo());
						player->renderParticles = true;
						player->doubleUp = true;
						player->AddScore(100 * (player->GetBonus() + 1));
						aMaster.playSample("RightCrystal");
					}
				}
				else
				{
					if (player->GetImmortalPortal() == true)
					{
						if (player->HasColor(item->GetColor()))
						{
							player->AddCombo(1);
							player->AddBonus(1);
							player->AddScore(100);
						}
						player->AddCombo(false);
						player->AddPower(player->GetCombo());
						player->renderParticles = true;
						player->doubleUp = true;
						aMaster.playSample("boing");
					}
					else
					{
						if (!player->HasColor(item->GetColor()))
						{
							player->RemoveBonus();
							player->RemovePower(0);
							player->RemoveCombo();
							player->AddScore(-20);
							aMaster.playSample("WrongCrystal");
						}
					}
					aMaster.playSample("WrongColor");
				}
				break;
			}
			default:
				break;
		}
		item->SetAlive(false);
		if (item->GetChild())
			item->GetChild()->SetAlive(false);

	}
}

void EntityHandler::KillAllMapItems()
{
	for (std::map<Shader*, std::list<Entity*>>::iterator ent = entities.begin(); ent != entities.end(); ++ent)
	{
		for (std::list<Entity*>::iterator i = ent->second.begin(); i != ent->second.end(); ++i)
		{
		
			if (dynamic_cast<MapItem*> (*i))
				(*i)->SetAlive(false);
		}
	}
}