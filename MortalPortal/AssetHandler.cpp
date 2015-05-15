#include "AssetHandler.h"

AssetHandler::AssetHandler()
{
}

AssetHandler::~AssetHandler()
{
	for (std::map<std::string, Geometry*>::iterator i = geometry.begin(); i != geometry.end(); ++i)
	{
		delete i->second;
	}
	for (std::map<std::string, Material*>::iterator i = material.begin(); i != material.end(); ++i)
	{
		delete i->second;
	}
}

LightL* AssetHandler::GetLight(ID3D11Device* device, std::string filename)
{
	LightL* returnValue = nullptr;

	if (light.find(filename) != light.end())
	{
		returnValue = light[filename];
	}
	else
	{
		LoadFile(device, filename);
		returnValue = light[filename];
	}
	if (returnValue == nullptr)
	{
		throw std::runtime_error("Failed to get pointer to Light in: " + filename);
	}
	return returnValue;

}

Geometry* AssetHandler::GetGeometry(ID3D11Device* device, std::string filename)
{
	Geometry* returnValue = nullptr;
	if (geometry.find(filename) != geometry.end()) //Geometry is found
	{
		returnValue = geometry[filename];
	}
	else
	{
		LoadFile(device, filename);
		returnValue = geometry[filename];
	}
	if (returnValue == nullptr)
	{
		throw std::runtime_error("Failed to get pointer to Geometry in: " + filename);
	}
	return returnValue;
}

Material* AssetHandler::GetMaterial(ID3D11Device* device, std::string filename, std::string materialName)
{
	Material* returnValue = nullptr;

	if (material.find(filename + materialName) != material.end()) //Material is found
	{
		returnValue = material[filename + materialName];
	}
	else
	{
		LoadFile(device, filename);
		returnValue = material[filename + materialName];
	}
	if (returnValue == nullptr)
	{
		throw std::runtime_error("Failed to get pointer to Material: " + materialName + "\nIn file: " + filename);
	}
	return returnValue;
}

Material* AssetHandler::GetMaterial(ID3D11Device* device, std::string diffuse_map, std::string normal_map, float normal_depth, DirectX::XMFLOAT3 specular, float specular_factor, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 transparency_color, DirectX::XMFLOAT3 incandescence)
{
	Material* returnValue = nullptr;

	if (material.find(diffuse_map + normal_map) != material.end()) //Material is found
	{
		returnValue = material[diffuse_map + normal_map];
	}
	else
	{
		returnValue = new Material(device, textureHandler.LoadTexture(diffuse_map, device), (normal_map.length() != 0 ? textureHandler.LoadTexture(normal_map, device) : nullptr), normal_depth, specular, specular_factor, ambient, diffuse, transparency_color, incandescence);
		material[diffuse_map + normal_map] = returnValue;
	}
	return returnValue;
}

void AssetHandler::LoadFile(ID3D11Device* device, std::string filename)
{


	Importer importer;
	bool importResult = importer.importFile(filename);
	if (!importResult)
	{
		throw std::runtime_error("Failed to import file: " + filename);
	}

	const ImporterTransform* transforms = importer.getTransform();

	const ImporterLight* light = importer.getLight();
	for (unsigned int i = 0; i < importer.getNumLights(); i++)
	{
		const ImporterLight& thisLight = light[i];

		bool isDir = true;
		if (thisLight.dType == ePoint)
		{
			isDir = false;
		}
		
		this->light[filename] = new LightL(device, DirectX::XMFLOAT3(1.0f, 0.0f, 0.5f), isDir, DirectX::XMFLOAT3(thisLight.color[0], thisLight.color[1], thisLight.color[2]));
	}


	if (importer.getNumMeshes() != 0)
	{
		Geometry* returnValue = nullptr;
		ID3D11Buffer* vertexBuffer;
		const ImporterMesh* mesh = importer.getMesh();

		unsigned int numVerticies = 0;
		vector<VertexPositionTexCoordNormalBinormalTangent> vertexVec;

		for (unsigned int meshID = 0; meshID < importer.getNumMeshes(); meshID++)
		{
			numVerticies += mesh[meshID].indice_count;
		}

		vertexVec.reserve(numVerticies);
		if (!strcmp(filename.c_str(), "assets/Portal_scaled.bin"))
			int toto = 0;
		for (unsigned int meshID = 0; meshID < importer.getNumMeshes(); meshID++)
		{
			constructVerticies(importer, mesh[meshID], vertexVec);
		}

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = numVerticies * sizeof(VertexPositionTexCoordNormalBinormalTangent);

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = vertexVec.data();
		HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create vertex buffer");
		}

		//Copy collision data
		Collision* objectCollision = new Collision();
		const ImporterNurb* spheres = importer.getNurb();

		for (unsigned int i = 0; i < importer.getNumNurbs(); i++)
		{
			DirectX::XMFLOAT3 position;
			float radius;
			const ImporterNurb& thisSphere = spheres[i];
			for (unsigned int k = 0; k < thisSphere.numberOfParent; k++)
			{
				DirectX::XMMATRIX TranslateTot = DirectX::XMMatrixIdentity();

				int parent = transforms[thisSphere.ParentID[k]].parentID;
				while (parent != -1)
				{
					double pos[3];
					double rotD[4];
					double scaleD[3];

					memcpy(pos, transforms[parent].position, sizeof(double)* 3);
					memcpy(rotD, transforms[parent].rotation, sizeof(double)* 4);
					memcpy(scaleD, transforms[parent].scale, sizeof(double)* 3);

					DirectX::XMMATRIX Translate = DirectX::XMMatrixAffineTransformation(DirectX::XMVectorSet(
						1 / (float)scaleD[0], 1 / (float)scaleD[1], 1 / (float)scaleD[2], 1.0f),
						DirectX::XMVectorSet(0, 0, 0, 1),
						DirectX::XMQuaternionInverse(DirectX::XMVectorSet((float)rotD[0], (float)rotD[1], (float)rotD[2], (float)rotD[3])),
						DirectX::XMVectorSet((float)pos[0], -(float)pos[1], (float)pos[2], 1.0f));

					TranslateTot = Translate * TranslateTot;

					parent = transforms[parent].parentID;
				}

				const ImporterTransform& ParentTarnsform = transforms[thisSphere.ParentID[k]];

				DirectX::XMVECTOR PosVec = DirectX::XMVectorSet((float)ParentTarnsform.position[0], (float)ParentTarnsform.position[1], (float)ParentTarnsform.position[2], 1.0f);
				PosVec = DirectX::XMVector3Transform(PosVec, DirectX::XMMatrixInverse(nullptr, TranslateTot));
				DirectX::XMFLOAT3 tmp3;
				DirectX::XMStoreFloat3(&tmp3, PosVec);

				position.x = tmp3.x;
				position.y = tmp3.y;
				position.z = tmp3.x;
				radius = spheres[i].radius;

				objectCollision->spheres.push_back(CollisionSphere(position, radius, ParentTarnsform.name));
			}
		}


		geometry[filename] = new Geometry(vertexBuffer, numVerticies, objectCollision);
	}

	const ImporterMaterial* materials = importer.getMaterial();
	for (unsigned int i = 0; i < importer.getNumMaterials(); i++)
	{

		ID3D11ShaderResourceView* diffuseMap = nullptr;
		ID3D11ShaderResourceView* normalMap = nullptr;

		const ImporterMaterial& thisMaterial = materials[i];

		if (thisMaterial.duffuse_map_length > 0)
		{
			diffuseMap = textureHandler.LoadTexture(thisMaterial.diffuse_map, device);
		}

		if (thisMaterial.normal_map_length > 0)
		{
			normalMap = textureHandler.LoadTexture(thisMaterial.normal_map, device);
		}

		material[filename + thisMaterial.name] = new Material(	device, diffuseMap, normalMap, thisMaterial.normal_depth, 
																DirectX::XMFLOAT3(thisMaterial.specular[0], thisMaterial.specular[1], thisMaterial.specular[2]), 
																thisMaterial.specular_factor, 
																DirectX::XMFLOAT3(thisMaterial.ambient[0], thisMaterial.ambient[1], thisMaterial.ambient[2]), 
																DirectX::XMFLOAT3(thisMaterial.diffuse[0], thisMaterial.diffuse[1], thisMaterial.diffuse[2]),
																DirectX::XMFLOAT3(thisMaterial.transparency_color[0], thisMaterial.transparency_color[1], thisMaterial.transparency_color[2]),
																DirectX::XMFLOAT3(thisMaterial.incandescence[0], thisMaterial.incandescence[1], thisMaterial.incandescence[2])
															 );
	}

}

void AssetHandler::constructVerticies(Importer& importer,const ImporterMesh& mesh, vector<VertexPositionTexCoordNormalBinormalTangent>& vertexBuffer)
{
	const ImporterTransform* transforms = importer.getTransform();

	for (unsigned int i = 0; i < mesh.transform_count; i++)
	{
		DirectX::XMMATRIX TranslateTot = DirectX::XMMatrixIdentity();

		int parent = mesh.transform_Id[i];
		while (parent != -1)
		{
			double pos[3];
			double rotD[4];
			double scaleD[3];

			memcpy(pos, transforms[parent].position, sizeof(double) * 3);
			memcpy(rotD, transforms[parent].rotation, sizeof(double) * 4);
			memcpy(scaleD, transforms[parent].scale, sizeof(double) * 3);

			DirectX::XMMATRIX Translate = DirectX::XMMatrixAffineTransformation( DirectX::XMVectorSet(
				1 / (float)scaleD[0], 1 / (float)scaleD[1], 1 / (float)scaleD[2], 1.0f),
				DirectX::XMVectorSet(0, 0, 0, 1), 
				DirectX::XMQuaternionInverse(DirectX::XMVectorSet((float)rotD[0], (float)rotD[1], (float)rotD[2], (float)rotD[3])),
				DirectX::XMVectorSet((float)pos[0], -(float)pos[1], (float)pos[2], 1.0f));

			TranslateTot = Translate * TranslateTot;
			
			parent = transforms[parent].parentID;
		}

		for (unsigned int j = 0; j < mesh.indice_count; j++)
		{
			VertexPositionTexCoordNormalBinormalTangent vertex;

			unsigned int positionID = mesh.vertices[j].position;
			unsigned int uvID = mesh.vertices[j].uv;
			unsigned int normalID = mesh.vertices[j].normal;

			DirectX::XMVECTOR PosVec = DirectX::XMVectorSet((float)mesh.position[positionID * 3 + 0], (float)mesh.position[positionID * 3 + 1], (float)mesh.position[positionID * 3 + 2], 1.0f);
			PosVec = DirectX::XMVector3Transform(PosVec, DirectX::XMMatrixInverse(nullptr, TranslateTot));
			DirectX::XMFLOAT3 tmp3;
			DirectX::XMStoreFloat3(&tmp3, PosVec);
			vertex.position[0] = tmp3.x;// *scale[0];
			vertex.position[1] = tmp3.y;// *scale[1];
			vertex.position[2] = tmp3.z;// *scale[2];
			vertex.texCoord[0] = mesh.uv[uvID * 2];
			vertex.texCoord[1] = mesh.uv[uvID * 2 + 1];
			vertex.normal[0] = (float)mesh.normal[normalID * 3 + 0];
			vertex.normal[1] = (float)mesh.normal[normalID * 3 + 1];
			vertex.normal[2] = (float)mesh.normal[normalID * 3 + 2];
			vertex.biNormal[0] = (float)mesh.bi_tangent[j * 3 + 0];
			vertex.biNormal[1] = (float)mesh.bi_tangent[j * 3 + 1];
			vertex.biNormal[2] = (float)mesh.bi_tangent[j * 3 + 2];
			vertex.tangent[0] = (float)mesh.tangent[j * 3 + 0];
			vertex.tangent[1] = (float)mesh.tangent[j * 3 + 1];
			vertex.tangent[2] = (float)mesh.tangent[j * 3 + 2];
			vertexBuffer.push_back(vertex);
		}
	}
}