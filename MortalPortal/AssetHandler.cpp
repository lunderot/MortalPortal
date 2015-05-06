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
		returnValue = new Material(textureHandler.LoadTexture(diffuse_map, device), (normal_map.length() != 0 ? textureHandler.LoadTexture(normal_map, device) : nullptr), normal_depth, specular, specular_factor, ambient, diffuse, transparency_color, incandescence);
		material[diffuse_map + normal_map] = returnValue;
	}
	return returnValue;
}

void AssetHandler::LoadFile(ID3D11Device* device, std::string filename)
{
	Geometry* returnValue = nullptr;
	ID3D11Buffer* vertexBuffer;

	Importer importer;
	bool importResult = importer.importFile(filename);
	if (!importResult)
	{
		throw std::runtime_error("Failed to import file: " + filename);
	}

	unsigned int numVerticies = 0;
	vector<VertexPositionTexCoordNormalBinormalTangent> vertexVec;

	for (unsigned int meshID = 0; meshID < importer.getNumMeshes(); meshID++)
	{
		numVerticies += importer.getMesh(meshID)->indice_count;
	}

	vertexVec.reserve(numVerticies);

	for (unsigned int meshID = 0; meshID < importer.getNumMeshes(); meshID++)
	{			
		constructVerticies(importer, meshID, vertexVec);
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

	for (unsigned int i = 0; i < importer.getNumNurbs(); i++)
	{
		DirectX::XMFLOAT3 position;
		float radius;
		const Nurb* spheres = importer.getNurb(i);

		for (unsigned int j = 0; j < spheres->numberOfParent; j++)
		{
			int parentID = spheres->parentID[j];

			const Transform* ParentTarnsform = importer.getTransform(parentID);

			position.x = ParentTarnsform->position[1];
			position.y = ParentTarnsform->position[1];
			position.z = ParentTarnsform->position[1];
			radius = spheres[i].radius;

			objectCollision->spheres.push_back(CollisionSphere(position, radius, ParentTarnsform->name));
		}
	}

	geometry[filename] = new Geometry(vertexBuffer, numVerticies, objectCollision);


	for (unsigned int i = 0; i < importer.getNumMaterials(); i++)
	{

		const MaterialData* thisMaterial = importer.getMaterial(i);
		if (thisMaterial->duffuse_map_length > 0)
		{
			//Convert char* to LPCWSTR
			std::string tmp = thisMaterial->diffuse_map;

			material[filename + importer.getMaterial(i)->node_name] = new Material(textureHandler.LoadTexture(tmp, device));
		}
	}

}

void AssetHandler::constructVerticies(Importer& importer, unsigned int meshID, vector<VertexPositionTexCoordNormalBinormalTangent>& vertexBuffer)
{
	VertexPositionTexCoordNormalBinormalTangent vertex;
	int heh = importer.getMesh(meshID)->transform_count;
	for (int i = 0; i < importer.getMesh(meshID)->transform_count; i++)
	{
		//float rotation[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float scale[3] = { 1.0f, 1.0f, 1.0f };
		float position[3] = { 0.0f, 0.0f, 0.0f };

		int parent = importer.getMesh(meshID)->transform_Id[i];
		while (parent != -1)
		{
			double pos[3];
			double rotD[4];
			double scaleD[3];

			memcpy(pos, importer.getTransform(parent)->position, sizeof(double) * 3);
			memcpy(rotD, importer.getTransform(parent)->rotation, sizeof(double) * 4);
			memcpy(scaleD, importer.getTransform(parent)->scale, sizeof(double) * 3);

			float rot[4];

			rot[0] = (float)rotD[0];
			rot[1] = (float)rotD[1];
			rot[2] = (float)rotD[2];
			rot[3] = (float)rotD[3];

			position[0] += (float)pos[0];
			position[1] += (float)pos[1];
			position[2] += (float)pos[2];

			DirectX::XMVECTOR Q1, Q2;
			Q1 = DirectX::XMVectorSet(rot[0], rot[1], rot[2], rot[3]);
			//Q2 = DirectX::XMVectorSet(rotation[0], rotation[1], rotation[2], rotation[3]);

			//Q2 = DirectX::XMQuaternionMultiply(Q1, Q2);

			DirectX::XMVECTOR posVec = DirectX::XMVectorSet(position[0], position[0], position[0], 1.0f);
			DirectX::XMQuaternionMultiply(DirectX::XMQuaternionInverse(Q1), posVec);

			DirectX::XMFLOAT3 tmp3;
			DirectX::XMStoreFloat3(&tmp3, posVec);
			position[0] += tmp3.x;
			position[1] += tmp3.y;
			position[2] += tmp3.z;

			position[0] *= scaleD[0];
			position[1] *= scaleD[1];
			position[2] *= scaleD[2];

			parent = importer.getTransform(parent)->parentID;
		}

		for (unsigned int j = 0; j < importer.getMesh(meshID)->indice_count; j++)
		{
			unsigned int positionID = importer.getMesh(meshID)->vertices[j].position;
			unsigned int uvID = importer.getMesh(meshID)->vertices[j].uv;
			unsigned int normalID = importer.getMesh(meshID)->vertices[j].normal;

			vertex.position[0] = (float)importer.getMesh(meshID)->position[positionID][0] + (float)position[0];
			vertex.position[1] = (float)importer.getMesh(meshID)->position[positionID][1] + (float)position[1];
			vertex.position[2] = (float)importer.getMesh(meshID)->position[positionID][2] + (float)position[2];

			memcpy(vertex.texCoord, importer.getMesh(meshID)->uv[uvID], sizeof(float) * 2);
			vertex.normal[0] = (float)importer.getMesh(meshID)->normal[normalID][0];
			vertex.normal[1] = (float)importer.getMesh(meshID)->normal[normalID][1];
			vertex.normal[2] = (float)importer.getMesh(meshID)->normal[normalID][2];
			vertex.biNormal[0] = (float)importer.getMesh(meshID)->bi_tangent[normalID][0];
			vertex.biNormal[1] = (float)importer.getMesh(meshID)->bi_tangent[normalID][1];
			vertex.biNormal[2] = (float)importer.getMesh(meshID)->bi_tangent[normalID][2];
			vertex.tangent[0] = (float)importer.getMesh(meshID)->tangent[normalID][0];
			vertex.tangent[1] = (float)importer.getMesh(meshID)->tangent[normalID][1];
			vertex.tangent[2] = (float)importer.getMesh(meshID)->tangent[normalID][2];
			vertexBuffer.push_back(vertex);
		}
	}
}