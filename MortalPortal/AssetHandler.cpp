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

Material* AssetHandler::GetMaterial(ID3D11Device* device, std::string filename, std::string materilName)
{
	Material* returnValue = nullptr;

	if (material.find(filename + materilName) != material.end()) //Material is found
	{
		returnValue = material[filename + materilName];
	}
	else
	{
		LoadFile(device, filename);
		returnValue = material[filename + materilName];
	}
	if (returnValue == nullptr)
	{
		throw std::runtime_error("Failed to get pointer to Material: " + materilName + "\nIn file: " + filename);
	}
	return returnValue;
}

Material* AssetHandler::GetMaterial(ID3D11Device* device, std::string filename)
{
	Material* returnValue = nullptr;


	if (material.find(filename) != material.end()) //Material is found
	{
		returnValue = material[filename];
	}
	else
	{
		returnValue = new Material(textureHandler.LoadTexture(filename, device));
		material[filename] = returnValue;
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

	const ImporterMesh* mesh = importer.getMesh();

	unsigned int numVerticies = 0;
	vector<VertexPositionTexCoordNormalBinormalTangent> vertexVec;

	for (unsigned int meshID = 0; meshID < importer.getNumMeshes(); meshID++)
	{
		numVerticies += mesh[meshID].indice_count;
	}

	vertexVec.reserve(numVerticies);

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
	const ImporterTransform* transforms = importer.getTransform();
	for (unsigned int i = 0; i < importer.getNumNurbs(); i++)
	{
		DirectX::XMFLOAT3 position;
		float radius;
		const ImporterNurb* spheres = importer.getNurb();

		for (unsigned int j = 0; j < importer.getNumNurbs(); j++)
		{
			const ImporterNurb& thisSphere = spheres[j];
			for (unsigned int k = 0; k < thisSphere.numberOfParent; k++)
			{

				int parentID = thisSphere.ParentID[k];

				const ImporterTransform& ParentTarnsform = transforms[parentID];

				position.x = (float)ParentTarnsform.position[0];
				position.y = (float)ParentTarnsform.position[1];
				position.z = (float)ParentTarnsform.position[2];
				radius = spheres[i].radius;

				objectCollision->spheres.push_back(CollisionSphere(position, radius, ParentTarnsform.name));
			}
		}
	}

	geometry[filename] = new Geometry(vertexBuffer, numVerticies, objectCollision);

	const ImporterMaterial* materials = importer.getMaterial();
	for (unsigned int i = 0; i < importer.getNumMaterials(); i++)
	{

		const ImporterMaterial& thisMaterial = materials[i];
		if (thisMaterial.duffuse_map_length > 0)
		{
			//Convert char* to LPCWSTR
			std::string tmp = thisMaterial.diffuse_map;

			material[filename + thisMaterial.name] = new Material(textureHandler.LoadTexture(tmp, device));
		}
	}

}

void AssetHandler::constructVerticies(Importer& importer,const ImporterMesh& mesh, vector<VertexPositionTexCoordNormalBinormalTangent>& vertexBuffer)
{
	VertexPositionTexCoordNormalBinormalTangent vertex;
	const ImporterTransform* transforms = importer.getTransform();

	for (unsigned int i = 0; i < mesh.transform_count; i++)
	{
		//float rotation[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float scale[3] = { 1.0f, 1.0f, 1.0f };
		float position[3] = { 0.0f, 0.0f, 0.0f };

		int parent = mesh.transform_Id[i];
		while (parent != -1)
		{
			double pos[3];
			double rotD[4];
			double scaleD[3];

			memcpy(pos, transforms[parent].position, sizeof(double) * 3);
			memcpy(rotD, transforms[parent].rotation, sizeof(double) * 4);
			memcpy(scaleD, transforms[parent].scale, sizeof(double) * 3);

			float rot[4];

			rot[0] = (float)rotD[0];
			rot[1] = (float)rotD[1];
			rot[2] = (float)rotD[2];
			rot[3] = (float)rotD[3];

			position[0] += (float)pos[0];
			position[1] += (float)pos[1];
			position[2] += (float)pos[2];

			position[0] *= (float)scaleD[0];
			position[1] *= (float)scaleD[1];
			position[2] *= (float)scaleD[2];

			DirectX::XMVECTOR Q1;
			Q1 = DirectX::XMVectorSet(rot[0], rot[1], rot[2], rot[3]);
			//Q2 = DirectX::XMVectorSet(rotation[0], rotation[1], rotation[2], rotation[3]);

			//Q2 = DirectX::XMQuaternionMultiply(Q1, Q2);

			DirectX::XMVECTOR posVec = DirectX::XMVectorSet(position[0], position[1], position[2], 1.0f);
			DirectX::XMQuaternionMultiply(DirectX::XMQuaternionInverse(Q1), posVec);

			DirectX::XMFLOAT3 tmp3;
			DirectX::XMStoreFloat3(&tmp3, posVec);
			position[0] = tmp3.x;
			position[1] = tmp3.y;
			position[2] = tmp3.z;

			//position[0] *= (float)scaleD[0];
			//position[1] *= (float)scaleD[1];
			//position[2] *= (float)scaleD[2];

			parent = transforms[parent].parentID;
		}

		for (unsigned int j = 0; j < mesh.indice_count; j++)
		{
			unsigned int positionID = mesh.vertices[j].position;
			unsigned int uvID = mesh.vertices[j].uv;
			unsigned int normalID = mesh.vertices[j].normal;

			vertex.position[0] = ((float)mesh.position[positionID * 3 + 0] + (float)position[0]);// *scale[0];
			vertex.position[1] = ((float)mesh.position[positionID * 3 + 1] + (float)position[1]);// *scale[1];
			vertex.position[2] = ((float)mesh.position[positionID * 3 + 2] + (float)position[2]);// *scale[2];
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