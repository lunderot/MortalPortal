#pragma once

#include <string>
#include <map>

#include "Geometry.h"
#include "Material.h"
#include "Shader.h"

#include "Importer.h"
#include "TextureHandler.h"


struct VertexPositionTexCoordNormalBinormalTangent
{
	float position[3];
	float texCoord[2];
	float normal[3];
	float biNormal[3];
	float tangent[3];
};

class AssetHandler
{
private:
	std::map<std::string, Geometry*> geometry;
	std::map<std::string, Material*> material;
	TextureHandler textureHandler;
public:
	AssetHandler();
	~AssetHandler();

	Geometry* GetGeometry(ID3D11Device* device, std::string filename);
	Material* GetMaterial(ID3D11Device* device, std::string filename, std::string materilName);
	Material* GetMaterial(ID3D11Device* device, std::string filename);

private:
	//Geometry* LoadGeometry(ID3D11Device* device, std::string filename);

	void LoadFile(ID3D11Device* device, std::string filename);
	void AssetHandler::constructVerticies(Importer& importer, unsigned int meshID, vector<VertexPositionTexCoordNormalBinormalTangent>& vertexBuffer);
};

