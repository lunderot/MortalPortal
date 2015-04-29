#ifndef IMPORTER_H
#define IMPORTER_H
#include "CommonDeclaration.h"
#include <string>

//struct BoundingSphere
//{
//	BoundingSphere()
//	{
//		position[0] = 0.0;
//		position[1] = 0.0;
//		position[2] = 0.0;
//		radius = 0.0f;
//	};
//	string name;
//	double position[3];
//	float radius;
//};

//struct ModelData
//{
//	ModelData()
//	{
//		MeshID = 0;
//		MaterialID = 0;
//		position[0] = 0.0;
//		position[1] = 0.0;
//		position[2] = 0.0;
//		rotation[0] = 0.0f;
//		rotation[1] = 0.0f;
//		rotation[2] = 0.0f;
//		rotation[3] = 1.0f;
//		scale[0] = 1.0;
//		scale[1] = 1.0;
//		scale[2] = 1.0;
//	};
//	string name;
//	unsigned int MeshID;
//	double position[3];
//	float rotation[4];
//	double scale[3];
//};

struct group
{

};

using namespace std;

class Importer
{
public:
	Importer();
	~Importer();

	bool importFile(string);

	unsigned int getNumTransforms() const;
	unsigned int getNumMaterials() const;
	unsigned int getNumCameras() const;
	unsigned int getNumLights() const;
	unsigned int getNumMeshes() const;
	unsigned int getNumNurbs() const;

	//const BoundingSphere* getBoundingSphere() const;

	//unsigned int numGroups();
	//const ModelData* getModels() const;

	const Transform* getTransform(unsigned int ID) const;
	const meshStruct* getMesh(unsigned int ID) const;
	const camera* getCamera(unsigned int ID) const;
	const MaterialData* getMaterial(unsigned int ID) const;
	const Light* getLight(unsigned int ID) const;
	const Joint* getJoint(unsigned int ID) const;
	const Nurb* getNurb(unsigned int ID) const;

private:
	//Allt här under lär bli private
	bool extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);

	bool extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractJoint(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractNurb(unsigned int& offset, char* fileData, unsigned int& fileSize);

	//bool constructModels();
	//bool constructSpere();
	//ModelData* models;
	//BoundingSphere* spheres;

	Header headers;

	Transform* transforms;
	meshStruct* meshes;
	camera* cameras;
	MaterialData* materials;
	Light* lights;
	Joint* joints;
	Nurb* nurbs;
};

#endif