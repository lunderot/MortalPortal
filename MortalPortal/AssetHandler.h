#pragma once

#include <string>
#include <map>

#include "Geometry.h"
#include "Material.h"
#include "Shader.h"

#include "Importer.h"
#include "TextureHandler.h"

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
	Material* GetMaterial(ID3D11Device* device, std::string filename);

private:
	Geometry* LoadGeometry(ID3D11Device* device, std::string filename);
	Material* LoadMaterial(std::string filename);
};

