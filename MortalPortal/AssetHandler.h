#pragma once

#include <string>
#include <map>

#include "Geometry.h"
#include "Importer.h"

class AssetHandler
{
private:
	std::map<std::string, Geometry*> geometry;
public:
	AssetHandler();
	~AssetHandler();

	Geometry* GetGeometry(ID3D11Device* device, std::string filename);
	Geometry* LoadGeometry(ID3D11Device* device, std::string filename);
};

