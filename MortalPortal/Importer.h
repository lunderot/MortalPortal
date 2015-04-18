#pragma once

#include "CommonDeclaration.h"
#include <string>

struct VertexPositionTexCoordNormalBinormalTangent
{
	float position[3];
	float texCoord[2];
	float normal[3];
	float biNormal[3];
	float tangent[3];
};

struct Model
{
	
	unsigned int MeshID;
	unsigned int MaterialID;
	double position[3];
	double rotation[4];
	double scale[3];
};

using namespace std;

class Importer
{
public:
	Importer();
	~Importer();
	
	bool importFile(string);

	unsigned int getNumMaterials() const;
	//unsigned int numCameras();
	//unsigned int numLights();
	unsigned int getNumMeshes() const;
	unsigned int getNumModels() const;
	unsigned int getMeshVertexCount(unsigned int meshID) const;

	//unsigned int numGroups();
	const Model* getModels() const;
	const Material* getMatrials() const;
	const VertexPositionTexCoordNormalBinormalTangent* getMesh(unsigned int meshID) const;

private:
//Allt här under lär bli private
	bool extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractTransformHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMeshHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractCameraHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMaterialHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractLightHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);

	bool extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize);

	bool constructVerticiesAndGeometry();
	bool constructModels();
	VertexPositionTexCoordNormalBinormalTangent** meshGeometry;
	unsigned int numModels;
	Model* models;

	Header headers;

	TransformHeader* transformHeaders;
	MeshHeader* meshHeaders;
	CameraHeader* cameraHeaders;
	MaterialHeader* materialHeaders;
	LightHeader* lightHeaders;

	Transform* transforms;
	meshStruct* meshes;
	camera* cameras;
	Material* materials;
	Light* lights;
};
