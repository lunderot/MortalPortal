#include "Importer.h"
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

Importer::Importer()
{
	meshGeometry = nullptr;

	transforms = nullptr;
	meshes = nullptr;
	cameras = nullptr;
	materials = nullptr;
	lights = nullptr;
	nurbs = nullptr;
};

Importer::~Importer()
{
	for (unsigned int i = 0; i < headers.mesh_count; i++)
		delete[] meshGeometry[i];
	delete[] meshGeometry;

	delete[] transforms;
	delete[] meshes;
	delete[] cameras;
	delete[] materials;
	delete[] lights;
	delete[] nurbs;
};

bool Importer::importFile(string filePathAndName)
{
	char* fileData;
	ifstream in(filePathAndName.c_str(), ios_base::binary | ios_base::ate);

	if (!in.is_open())
		return false;
	unsigned int fileByteSize = in.tellg();
	in.seekg(0, ios_base::beg);

	fileData = new char[fileByteSize];
	in.read(fileData, fileByteSize);
	in.close();

	unsigned int offset = 0;

	if (!extractMainHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMaterials(offset, fileData, fileByteSize)) return false;

	if (!extractTransforms(offset, fileData, fileByteSize)) return false;

	if (!extractJoint(offset, fileData, fileByteSize)) return false;

	if (!extractCameras(offset, fileData, fileByteSize)) return false;

	if (!extractMeshes(offset, fileData, fileByteSize)) return false;

	if (!extractLights(offset, fileData, fileByteSize)) return false;

	if (!extractNurb(offset, fileData, fileByteSize)) return false;

	if (!constructVerticiesAndGeometry()) return false;

	if (!constructModels()) return false;

	if (!constructSpere()) return false;

	return true;
};

bool Importer::extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(Header) > fileSize - offset)
		return false;

	memcpy(&headers, &fileData[0], sizeof(Header));
	offset += sizeof(Header);

	return true;
}

bool Importer::extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	transforms = new Transform[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		if ((sizeof(Transform) - sizeof(char*)) > fileSize - offset)
			return false;

		memcpy(&transforms[i], &fileData[offset], sizeof(Transform) - sizeof(char*));
		offset += sizeof(Transform) - sizeof(char*) - 4;

		if (transforms[i].name_Length> fileSize - offset)
			return false;

		char* node_name = new char[transforms[i].name_Length + 1];
		node_name[transforms[i].name_Length] = '\0';
		transforms[i].name = node_name;

		memcpy((char*)transforms[i].name, &fileData[offset], transforms[i].name_Length);

		offset += transforms[i].name_Length;
	}

	return true;
}

bool Importer::extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize)
{

	unsigned int test1 = sizeof(meshStruct);
	unsigned int test2 = sizeof(unsigned int);
	unsigned int test3 = sizeof(bool);
	unsigned int test4 = sizeof(double**);
	unsigned int test5 = sizeof(float**);
	unsigned int test6 = sizeof(int*);
	unsigned int test7 = sizeof(Vertex*);
	unsigned int test8 = sizeof(const char*);


	meshes = new meshStruct[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		memcpy(&meshes[i], &fileData[offset], sizeof(meshStruct) - sizeof(char*) * 9 - 3);
		offset += sizeof(meshStruct) - sizeof(char*) * 9 + 1;

		meshes[i].position = new double*[meshes[i].position_count];
		for (unsigned int j = 0; j < meshes[i].position_count; j++)
		{
			meshes[i].position[j] = new double[3];
			double test[3];
			memcpy((char*)test, &fileData[offset], 3 * sizeof(double));
			meshes[i].position[j][0] = test[0];
			meshes[i].position[j][1] = test[1];
			meshes[i].position[j][2] = test[2];
			offset += 3 * sizeof(double);
		}

		meshes[i].uv = new float*[meshes[i].uv_count];
		for (unsigned int j = 0; j < meshes[i].uv_count; j++)
		{
			meshes[i].uv[j] = new float[2];

			memcpy((char*)meshes[i].uv[j], &fileData[offset], 2 * sizeof(float));
			offset += 2 * sizeof(float);
		}

		meshes[i].normal = new double*[meshes[i].normal_count];
		for (unsigned int j = 0; j < meshes[i].normal_count; j++)
		{
			meshes[i].normal[j] = new double[3];

			memcpy((char*)meshes[i].normal[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].tangent = new double*[meshes[i].tangent_count];
		for (unsigned int j = 0; j < meshes[i].tangent_count; j++)
		{
			meshes[i].tangent[j] = new double[3];

			memcpy((char*)meshes[i].tangent[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].bi_tangent = new double*[meshes[i].biTangent_count];
		for (unsigned int j = 0; j < meshes[i].biTangent_count; j++)
		{
			meshes[i].bi_tangent[j] = new double[3];

			memcpy((char*)meshes[i].bi_tangent[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].vertices = new Vertex[meshes[i].indice_count];
		meshes[i].material_Id = new int[meshes[i].material_count];
		meshes[i].transform_Id = new int[meshes[i].transform_count];
		char* name = new char[meshes[i].name_length + 1];
		name[meshes[i].name_length] = '\0';

		memcpy((char*)meshes[i].transform_Id, &fileData[offset], meshes[i].transform_count * sizeof(int));
		offset += meshes[i].transform_count * sizeof(int);

		memcpy((char*)meshes[i].material_Id, &fileData[offset], meshes[i].material_count * sizeof(int));
		offset += meshes[i].material_count * sizeof(int);

		memcpy((char*)meshes[i].vertices, &fileData[offset], meshes[i].indice_count * sizeof(Vertex));
		offset += meshes[i].indice_count * sizeof(Vertex);

		memcpy((char*)name, &fileData[offset], meshes[i].name_length);
		meshes[i].name = name;
		offset += meshes[i].name_length;

	}

	return true;
}

bool Importer::extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	cameras = new camera[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		memcpy(&cameras[i], &fileData[offset], sizeof(camera) - sizeof(char*) - sizeof(unsigned int*));
		offset += sizeof(camera) - sizeof(char*) - 4;

		cameras[i].parentID = new unsigned int[cameras[i].nrOfParents];
		memcpy(cameras[i].parentID, &fileData[offset], sizeof(int) * cameras[i].nrOfParents);
		offset += sizeof(unsigned int) * cameras[i].nrOfParents;

		char* node_name = new char[cameras[i].name_length + 1];
		node_name[cameras[i].name_length] = '\0';
		cameras[i].name = node_name;

		memcpy((char*)cameras[i].name, &fileData[offset], cameras[i].name_length);

		offset += cameras[i].name_length;

		cameras[i] = cameras[i];
	}

	return true;
}

bool Importer::extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	materials = new MaterialData[headers.material_count];
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		memcpy(&materials[i], &fileData[offset], sizeof(MaterialData) - sizeof(const char*) * 4);
		offset += sizeof(MaterialData) - sizeof(const char*) * 4;
		char* node_name = new char[materials[i].name_length + 1];
		node_name[materials[i].name_length] = '\0';

		materials[i].node_name = node_name;

		memcpy((char*)materials[i].node_name, &fileData[offset], materials[i].name_length);
		offset += materials[i].name_length;


		if (materials[i].duffuse_map_length != 0)
		{
			char* diffuse_map = new char[materials[i].duffuse_map_length + 1];
			diffuse_map[materials[i].duffuse_map_length] = '\0';

			memcpy(diffuse_map, &fileData[offset], materials[i].duffuse_map_length);
			offset += materials[i].duffuse_map_length;

			materials[i].diffuse_map = diffuse_map;
		}
		else
		{
			materials[i].diffuse_map = nullptr;
		}

		if (materials[i].normal_map_length != 0)
		{
			char* normal_map = new char[materials[i].normal_map_length + 1];
			normal_map[materials[i].normal_map_length] = '\0';

			memcpy(normal_map, &fileData[offset], materials[i].normal_map_length);
			offset += materials[i].normal_map_length;

			materials[i].normal_map = normal_map;
		}
		else
		{
			materials[i].normal_map = nullptr;
		}

		if (materials[i].specular_map_length != 0)
		{
			char* specular_map = new char[materials[i].specular_map_length + 1];
			specular_map[materials[i].specular_map_length] = '\0';

			memcpy(specular_map, &fileData[offset], materials[i].specular_map_length);
			offset += materials[i].specular_map_length;

			materials[i].specular_map = specular_map;
		}
		else
		{
			materials[i].specular_map = nullptr;
		}
	}

	return true;
}

bool Importer::extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	lights = new Light[headers.light_count];
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		memcpy(&lights[i], &fileData[offset], sizeof(Light) - sizeof(char*));
		offset += sizeof(Light) - sizeof(char*) - 4;

		char* name = new char[lights[i].name_Length + 1];
		name[lights[i].name_Length] = '\0';
		lights[i].name = name;

		memcpy((char*)lights[i].name, &fileData[offset], lights[i].name_Length);

		offset += lights[i].name_Length;
	}

	return true;
}

bool Importer::extractJoint(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	joints = new Joint[headers.joint_count];
	for (unsigned int i = 0; i < headers.joint_count; i++)
	{
		memcpy(&joints[i], &fileData[offset], sizeof(Joint) - sizeof(char*));
		offset += sizeof(Joint) - sizeof(char*) - 4;

		char* name = new char[joints[i].transform.name_Length + 1];
		name[joints[i].transform.name_Length] = '\0';
		joints[i].transform.name = name;

		memcpy((char*)joints[i].transform.name, &fileData[offset], joints[i].transform.name_Length);

		offset += joints[i].transform.name_Length;
	}

	return true;
}

bool Importer::extractNurb(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int test = sizeof(Nurb);
	nurbs = new Nurb[headers.nurb_count];
	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		memcpy(&nurbs[i], &fileData[offset], sizeof(Nurb) - sizeof(char*) - sizeof(int*));
		offset += sizeof(Nurb) - sizeof(char*) - sizeof(int*) + 4;

		nurbs[i].parentID = new int[nurbs[i].numberOfParent];
		memcpy(nurbs[i].parentID, &fileData[offset], nurbs[i].numberOfParent * sizeof(int));
		offset += nurbs[i].numberOfParent * sizeof(int);

		char* name = new char[nurbs[i].name_Length + 1];
		name[nurbs[i].name_Length] = '\0';
		nurbs[i].name = name;

		memcpy((char*)nurbs[i].name, &fileData[offset], nurbs[i].name_Length);

		offset += nurbs[i].name_Length;
	}

	return true;
}

bool Importer::constructVerticiesAndGeometry()
{
	meshGeometry = new VertexPositionTexCoordNormalBinormalTangent*[headers.mesh_count];

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		meshGeometry[i] = new VertexPositionTexCoordNormalBinormalTangent[meshes[i].indice_count];
		for (unsigned int j = 0; j < meshes[i].indice_count; j++)
		{
			unsigned int positionID = meshes[i].vertices[j].position;
			unsigned int uvID = meshes[i].vertices[j].uv;
			unsigned int normalID = meshes[i].vertices[j].normal;
			meshGeometry[i][j].position[0] = (float)meshes[i].position[positionID][0];
			meshGeometry[i][j].position[1] = (float)meshes[i].position[positionID][1];
			meshGeometry[i][j].position[2] = (float)meshes[i].position[positionID][2];
			memcpy(meshGeometry[i][j].texCoord, meshes[i].uv[uvID], sizeof(float) * 2);
			meshGeometry[i][j].normal[0] = (float)meshes[i].normal[normalID][0];
			meshGeometry[i][j].normal[1] = (float)meshes[i].normal[normalID][1];
			meshGeometry[i][j].normal[2] = (float)meshes[i].normal[normalID][2];
			meshGeometry[i][j].biNormal[0] = (float)meshes[i].bi_tangent[normalID][0];
			meshGeometry[i][j].biNormal[1] = (float)meshes[i].bi_tangent[normalID][1];
			meshGeometry[i][j].biNormal[2] = (float)meshes[i].bi_tangent[normalID][2];
			meshGeometry[i][j].tangent[0] = (float)meshes[i].tangent[normalID][0];
			meshGeometry[i][j].tangent[1] = (float)meshes[i].tangent[normalID][1];
			meshGeometry[i][j].tangent[2] = (float)meshes[i].tangent[normalID][2];
		}
	}

	return true;
}

bool Importer::constructModels()
{
	for (unsigned int i = 0; i < headers.mesh_count; i++)
		numModels += meshes[i].transform_count;

	models = new Model[numModels];

	int modelID = 0;

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		for (unsigned int j = 0; j < meshes[i].transform_count; j++)
		{
			int parent = meshes[i].transform_Id[j];

			while (parent != -1)
			{
				double pos[3];
				double rotD[4];
				double scale[3];

				memcpy(pos, transforms[parent].position, sizeof(double) * 3);
				memcpy(rotD, transforms[parent].rotation, sizeof(double) * 4);
				memcpy(scale, transforms[parent].scale, sizeof(double) * 3);

				float rot[4];

				rot[0] = (float)rotD[0];
				rot[1] = (float)rotD[1];
				rot[2] = (float)rotD[2];
				rot[3] = (float)rotD[3];


				models[modelID].position[0] += pos[0];
				models[modelID].position[1] += pos[1];
				models[modelID].position[2] += pos[2];

				DirectX::XMVECTOR Q1, Q2;
				Q1 = DirectX::XMVectorSet(rot[0], rot[1], rot[2], rot[3]);
				Q2 = DirectX::XMVectorSet(models[modelID].rotation[0], models[modelID].rotation[1], models[modelID].rotation[2], models[modelID].rotation[3]);

				Q2 = DirectX::XMQuaternionMultiply(Q1, Q2);


				DirectX::XMFLOAT4 tmp;
				DirectX::XMStoreFloat4(&tmp, Q2);
				models[modelID].rotation[0] = tmp.x;
				models[modelID].rotation[1] = tmp.y;
				models[modelID].rotation[2] = tmp.z;
				models[modelID].rotation[3] = tmp.w;


				models[modelID].scale[0] *= scale[0];
				models[modelID].scale[1] *= scale[1];
				models[modelID].scale[2] *= scale[2];

				parent = transforms[parent].parentID;
			}

			models[modelID].MeshID = i;
			models[modelID].MaterialID = j;

			modelID++;
		}
	}

	return true;
}

bool Importer::constructSpere()
{
	for (unsigned int i = 0; i < headers.nurb_count; i++)
		numSpheres += nurbs[i].numberOfParent;

	spheres = new BoundingSphere[numSpheres];

	unsigned int sphereID = 0;

	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		for (unsigned int j = 0; j < nurbs[i].numberOfParent; j++)
		{
			int parent = nurbs[i].parentID[j];

			while (parent != -1)
			{
				double pos[3];

				memcpy(pos, transforms[parent].position, sizeof(double) * 3);

				spheres[sphereID].position[0] += pos[0];
				spheres[sphereID].position[1] += pos[1];
				spheres[sphereID].position[2] += pos[2];

				parent = transforms[parent].parentID;;
			}
			spheres[sphereID].radius = nurbs[i].radius;
			sphereID++;
		}
	}

	return true;
}

const MaterialData* Importer::getMatrials() const
{
	return materials;
}

const VertexPositionTexCoordNormalBinormalTangent* Importer::getMesh(unsigned int meshID) const
{
	if (0 <= meshID && meshID < headers.mesh_count)
		return meshGeometry[meshID];
	else
		return nullptr;
}

unsigned int Importer::getNumBoundingSphere() const
{
	return numSpheres;
}

const BoundingSphere* Importer::getBoundingSphere() const
{
	if (numSpheres != 0)
		return spheres;
	else
		return nullptr;
}

const Model* Importer::getModels() const
{
	return models;
}

unsigned int Importer::getNumMaterials() const
{
	return headers.material_count;
}

unsigned int Importer::getNumMeshes() const
{
	return headers.mesh_count;
}

unsigned int Importer::getNumModels() const
{
	return numModels;
}

unsigned int Importer::getMeshVertexCount(unsigned int meshID) const
{
	if (0 <= meshID && meshID < headers.mesh_count)
		return meshes[meshID].indice_count;
	else
		return 0;
}