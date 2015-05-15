#pragma once

#include <string>
#include <map>

#include "Light.h"
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
	std::map<std::string, LightL*> light;
	std::map<std::string, Geometry*> geometry;
	std::map<std::string, Material*> material;
	TextureHandler textureHandler;
public:
	AssetHandler();
	~AssetHandler();

	LightL* GetLight(
		ID3D11Device* device,
		std::string filename);
	Geometry* GetGeometry(
		ID3D11Device* device, 
		std::string filename);
	Material* GetMaterial(
		ID3D11Device* device, 
		std::string filename, 
		std::string materialName);
	Material* GetMaterial(
		ID3D11Device* device, 
		std::string filename, 
		std::string normal_map, 
		float normal_depth,
		DirectX::XMFLOAT3 specular = DirectX::XMFLOAT3(0, 0, 0),
		float specular_factor = 0.0f,
		DirectX::XMFLOAT3 ambient = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 diffuse = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 transparency_color = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 incandescence = DirectX::XMFLOAT3(0, 0, 0),
		std::string specular_map = "");

private:
	//Geometry* LoadGeometry(ID3D11Device* device, std::string filename);

	void LoadFile(ID3D11Device* device, std::string filename);
	void constructVerticies(Importer& importer,const ImporterMesh& mesh, vector<VertexPositionTexCoordNormalBinormalTangent>& vertexBuffer);
};

