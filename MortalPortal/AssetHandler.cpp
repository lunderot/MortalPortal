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
		returnValue = LoadGeometry(device, filename);
		geometry[filename] = returnValue;
	}
	return returnValue;
}

Material* AssetHandler::GetMaterial(ID3D11Device* device, std::string filename)
{
	Material* returnValue = nullptr;

	//Convert string to LPCWSTR
	std::wstring stemp = std::wstring(filename.begin(), filename.end());
	LPCWSTR sw = stemp.c_str();

	if (material.find(filename) != material.end()) //Material is found
	{
		returnValue = material[filename];
	}
	else
	{
		returnValue = new Material(textureHandler.LoadTexture(sw, device));
		material[filename] = returnValue;
	}
	return returnValue;
}

Geometry* AssetHandler::LoadGeometry(ID3D11Device* device, std::string filename)
{
	Geometry* returnValue = nullptr;
	ID3D11Buffer* vertexBuffer;
	unsigned int meshID = 0;

	Importer importer;
	bool importResult = importer.importFile(filename);
	if (!importResult)
	{
		throw std::runtime_error("Failed to import file: " + filename);
	}

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = importer.getMeshVertexCount(meshID) * sizeof(VertexPositionTexCoordNormalBinormalTangent);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = importer.getMesh(meshID);
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}

	//Copy collision data
	Collision* objectCollision = new Collision();

	for (int i = 0; i < importer.getNumBoundingSphere(); i++)
	{
		DirectX::XMFLOAT3 position;
		float radius;
		const BoundingSphere* spheres = importer.getBoundingSphere();

		position.x = spheres[i].position[0];
		position.y = spheres[i].position[1];
		position.z = spheres[i].position[2];
		radius = spheres[i].radius;

		objectCollision->spheres.push_back(CollisionSphere(position, radius));
	}

	returnValue = new Geometry(vertexBuffer, importer.getMeshVertexCount(meshID), objectCollision);

	return returnValue;
}