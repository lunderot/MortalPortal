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

Geometry* AssetHandler::LoadGeometry(ID3D11Device* device, std::string filename)
{
	Geometry* returnValue = nullptr;
	ID3D11Buffer* vertexBuffer;
	unsigned int meshID = 0;

	Importer importer;
	importer.importFile(filename);

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

	returnValue = new Geometry(vertexBuffer, importer.getMeshVertexCount(meshID));

	return returnValue;
}