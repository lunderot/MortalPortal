#include "Importer.h"
#include <fstream>
#include <iostream>
// magnus is here ;)
Importer::Importer()
{
	meshGeometry = nullptr;
	transformHeaders = nullptr;
	meshHeaders = nullptr;
	cameraHeaders = nullptr;
	materialHeaders = nullptr;
	lightHeaders = nullptr;

	transforms = nullptr;
	meshes = nullptr;
	cameras = nullptr;
	materials = nullptr;
	lights = nullptr;
};

Importer::~Importer()
{
	for (unsigned int i = 0; i < headers.mesh_count; i++)
		delete[] meshGeometry[i];
	delete[] meshGeometry;
	delete[] transformHeaders;
	delete[] meshHeaders;
	delete[] cameraHeaders;
	delete[] materialHeaders;
	delete[] lightHeaders;

	delete[] transforms;
	delete[] meshes;
	delete[] cameras;
	delete[] materials;
	delete[] lights;
};

bool Importer::importFile(string filePathAndName)
{
	char* fileData;
	ifstream in(filePathAndName.c_str(), ios_base::binary|ios_base::ate);

	if (!in.is_open())
		return false;
	unsigned int fileByteSize = in.tellg();
	in.seekg(0, ios_base::beg);

	fileData = new char[fileByteSize];
	in.read(fileData, fileByteSize);
	in.close();

	unsigned int offset = 0;

	if (!extractMainHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMaterialHeader(offset, fileData, fileByteSize)) return false;

	if (!extractTransformHeader(offset, fileData, fileByteSize)) return false;

	if (!extractCameraHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMeshHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMaterials(offset, fileData, fileByteSize)) return false;

	if (!extractTransforms(offset, fileData, fileByteSize)) return false;

	if (!extractCameras(offset, fileData, fileByteSize)) return false;

	if (!extractMeshes(offset, fileData, fileByteSize)) return false;

	if (!constructVerticiesAndGeometry()) return false;

	if (!constructModels()) return false;

	return true;
};

bool Importer::extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(Header) > fileSize - offset)
		return false;

	Header extractedHeader;
	memcpy(&extractedHeader, &fileData[0], sizeof(Header));
	offset += sizeof(Header);
	headers = extractedHeader;

	return true;
}

bool Importer::extractTransformHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(TransformHeader) * headers.group_count > fileSize - offset)
		return false;

	transformHeaders = new TransformHeader[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		TransformHeader extractedTransformHeader;
		memcpy(&extractedTransformHeader, &fileData[offset], sizeof(TransformHeader));
		offset += sizeof(TransformHeader);
		transformHeaders[i] = extractedTransformHeader;
	}

	return true;
}

bool Importer::extractMeshHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(MeshHeader) * headers.mesh_count > fileSize - offset)
		return false;

	meshHeaders = new MeshHeader[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		MeshHeader extractedMesh;
		memcpy(&extractedMesh, &fileData[offset], sizeof(MeshHeader));
		offset += sizeof(MeshHeader);
		meshHeaders[i] = extractedMesh;
	}

	return true;
}

bool Importer::extractCameraHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(CameraHeader) * headers.camera_count > fileSize - offset)
		return false;

	cameraHeaders = new CameraHeader[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		CameraHeader extractedCamera;
		memcpy(&extractedCamera, &fileData[offset], sizeof(CameraHeader));
		offset += sizeof(CameraHeader);
		cameraHeaders[i] = extractedCamera;
	}

	return true;
}

bool Importer::extractMaterialHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(MaterialHeader) * headers.material_count > fileSize - offset)
		return false;

	materialHeaders = new MaterialHeader[headers.material_count];
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		MaterialHeader extractedMaterialHeader;
		memcpy(&extractedMaterialHeader, &fileData[offset], sizeof(MaterialHeader));
		offset += sizeof(MaterialHeader);
		materialHeaders[i] = extractedMaterialHeader;
	}
	
	return true;
}

bool Importer::extractLightHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	return true;
}

bool Importer::extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		readSize += (sizeof(Transform) - sizeof(char*)) + transformHeaders[i].name_Length;
	}

	if (readSize > fileSize - offset)
		return false;

	transforms = new Transform[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		Transform extractedTransform;
		memcpy(&extractedTransform, &fileData[offset], sizeof(Transform) - sizeof(char*));
		offset += sizeof(Transform) - sizeof(char*) - 4;

		char* node_name = new char[transformHeaders[i].name_Length + 1];
		node_name[transformHeaders[i].name_Length] = '\0';
		extractedTransform.name = node_name;

		memcpy((char*)extractedTransform.name, &fileData[offset], transformHeaders[i].name_Length);

		offset += transformHeaders[i].name_Length;

		transforms[i] = extractedTransform;
	}

	return true;
}

bool Importer::extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		readSize += meshHeaders[i].position_count * sizeof(double) * 3 + meshHeaders[i].uv_count * sizeof(float) + meshHeaders[i].normal_count * sizeof(double) * 3 +
					meshHeaders[i].tangent_count * sizeof(double) * 3 + meshHeaders[i].biTangent_count * sizeof(double) * 3 +
					meshHeaders[i].transform_count * sizeof(int) + meshHeaders[i].material_count * sizeof(int) + meshHeaders[i].indice_count * sizeof(Vertex) +
					meshHeaders[i].name_length;
	}

	if (readSize > fileSize - offset)
		return false;

	meshes = new meshStruct[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{

		meshStruct extractedMesh;

		extractedMesh.position.resize(meshHeaders[i].position_count);
		for (unsigned int j = 0; j < meshHeaders[i].position_count; j++)
		{
			extractedMesh.position[j].resize(3);

			memcpy((char*)extractedMesh.position[j].data(), &fileData[offset], extractedMesh.position[j].size() * sizeof(double));
			offset += extractedMesh.position[j].size() * sizeof(double);
		}

		extractedMesh.uv.resize(meshHeaders[i].uv_count);
		for (unsigned int j = 0; j < meshHeaders[i].uv_count; j++)
		{
			extractedMesh.uv[j].resize(2);

			memcpy((char*)extractedMesh.uv[j].data(), &fileData[offset], extractedMesh.uv[j].size() * sizeof(float));
			offset += extractedMesh.uv[j].size() * sizeof(float);
		}

		extractedMesh.normal.resize(meshHeaders[i].normal_count);
		for (unsigned int j = 0; j < meshHeaders[i].normal_count; j++)
		{
			extractedMesh.normal[j].resize(3);

			memcpy((char*)extractedMesh.normal[j].data(), &fileData[offset], extractedMesh.normal[j].size() * sizeof(double));
			offset += extractedMesh.normal[j].size() * sizeof(double);
		}

		extractedMesh.tangent.resize(meshHeaders[i].tangent_count);
		for (unsigned int j = 0; j < meshHeaders[i].tangent_count; j++)
		{
			extractedMesh.tangent[j].resize(3);

			memcpy((char*)extractedMesh.tangent[j].data(), &fileData[offset], extractedMesh.tangent[j].size() * sizeof(double));
			offset += extractedMesh.tangent[j].size() * sizeof(double);
		}

		extractedMesh.bi_tangent.resize(meshHeaders[i].biTangent_count);
		for (unsigned int j = 0; j < meshHeaders[i].biTangent_count; j++)
		{
			extractedMesh.bi_tangent[j].resize(3);

			memcpy((char*)extractedMesh.bi_tangent[j].data(), &fileData[offset], extractedMesh.bi_tangent[j].size() * sizeof(double));
			offset += extractedMesh.bi_tangent[j].size() * sizeof(double);
		}

		extractedMesh.vertices.resize(meshHeaders[i].indice_count);
		extractedMesh.material_Id.resize(meshHeaders[i].material_count);
		extractedMesh.transform_Id.resize(meshHeaders[i].transform_count);
		char* name = new char[meshHeaders[i].name_length + 1];
		name[meshHeaders[i].name_length] = '\0';

		memcpy((char*)extractedMesh.transform_Id.data(), &fileData[offset], meshHeaders[i].transform_count * sizeof(int));
		offset += meshHeaders[i].transform_count * sizeof(int);

		memcpy((char*)extractedMesh.material_Id.data(), &fileData[offset], meshHeaders[i].material_count * sizeof(int));
		offset += meshHeaders[i].material_count * sizeof(int);

		memcpy((char*)extractedMesh.vertices.data(), &fileData[offset], meshHeaders[i].indice_count * sizeof(Vertex));
		offset += meshHeaders[i].indice_count * sizeof(Vertex);

		memcpy((char*)name, &fileData[offset], meshHeaders[i].name_length);
		extractedMesh.name = name;
		offset += meshHeaders[i].name_length;

		meshes[i] = extractedMesh;
	}

	return true;
}

bool Importer::extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		readSize += (sizeof(camera) - sizeof(char*)) + cameraHeaders[i].name_length;
	}

	if (readSize > fileSize - offset)
		return false;


	cameras = new camera[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		camera extractCamera;
		memcpy(&extractCamera, &fileData[offset], sizeof(camera) - sizeof(char*));
		offset += sizeof(camera) - sizeof(char*) + 4;

		char* node_name = new char[cameraHeaders[i].name_length + 1];
		node_name[cameraHeaders[i].name_length] = '\0';
		extractCamera.name = node_name;

		memcpy((char*)extractCamera.name, &fileData[offset], cameraHeaders[i].name_length);

		offset += cameraHeaders[i].name_length;

		cameras[i] = extractCamera;
	}

	return true;
}

bool Importer::extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		readSize += (sizeof(Material) - sizeof(char*) * 4) +
			materialHeaders[i].name_length + materialHeaders[i].duffuse_map_length +
			materialHeaders[i].normal_map_length + materialHeaders[i].specular_map_length;
	}

	if (readSize > fileSize - offset)
		return false;


	materials = new Material[headers.material_count];
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		Material extractedMaterial;
		memcpy(&extractedMaterial, &fileData[offset], sizeof(Material) - sizeof(const char*) * 4);
		offset += sizeof(Material) - sizeof(const char*) * 4;
		char* node_name = new char[materialHeaders[i].name_length + 1];
		node_name[materialHeaders[i].name_length] = '\0';

		extractedMaterial.node_name = node_name;

		memcpy((char*)extractedMaterial.node_name, &fileData[offset], materialHeaders[i].name_length);
		offset += materialHeaders[i].name_length;


		if (materialHeaders[i].duffuse_map_length != 0)
		{
			char* diffuse_map = new char[materialHeaders[i].duffuse_map_length + 1];
			diffuse_map[materialHeaders[i].duffuse_map_length] = '\0';

			memcpy(diffuse_map, &fileData[offset], materialHeaders[i].duffuse_map_length);
			offset += materialHeaders[i].duffuse_map_length;

			extractedMaterial.diffuse_map = diffuse_map;
		}
		else
		{
			extractedMaterial.diffuse_map = nullptr;
		}

		if (materialHeaders[i].normal_map_length != 0)
		{
			char* normal_map = new char[materialHeaders[i].normal_map_length + 1];
			normal_map[materialHeaders[i].normal_map_length] = '\0';

			memcpy(normal_map, &fileData[offset], materialHeaders[i].normal_map_length);
			offset += materialHeaders[i].normal_map_length;

			extractedMaterial.normal_map = normal_map;
		}
		else
		{
			extractedMaterial.normal_map = nullptr;
		}

		if (materialHeaders[i].specular_map_length != 0)
		{
			char* specular_map = new char[materialHeaders[i].specular_map_length + 1];
			specular_map[materialHeaders[i].specular_map_length] = '\0';

			memcpy(specular_map, &fileData[offset], materialHeaders[i].specular_map_length);
			offset += materialHeaders[i].specular_map_length;

			extractedMaterial.specular_map = specular_map;
		}
		else
		{
			extractedMaterial.specular_map = nullptr;
		}

		materials[i] = extractedMaterial;
	}

	return true;
}

bool Importer::extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	return true;
}

bool Importer::constructVerticiesAndGeometry()
{
	meshGeometry = new VertexPositionTexCoordNormalBinormalTangent* [headers.mesh_count];

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		meshGeometry[i] = new VertexPositionTexCoordNormalBinormalTangent[meshHeaders[i].indice_count];
		for (unsigned int j = 0; j < meshHeaders[i].indice_count; j++)
		{
			unsigned int& positionID = meshes[i].vertices[j].position;
			unsigned int& uvID = meshes[i].vertices[j].uv;
			unsigned int& normalID = meshes[i].vertices[j].normal;
			memcpy(meshGeometry[i][j].position, meshes[i].position[positionID].data(), sizeof(double) * 3);
			memcpy(meshGeometry[i][j].texCoord, meshes[i].uv[uvID].data(), sizeof(double) * 3);
			memcpy(meshGeometry[i][j].normal, meshes[i].normal[normalID].data(), sizeof(double) * 3);
			memcpy(meshGeometry[i][j].biNormal, meshes[i].bi_tangent[normalID].data(), sizeof(double) * 3);
			memcpy(meshGeometry[i][j].tangent, meshes[i].tangent[normalID].data(), sizeof(double) * 3);
		}
	}

	return true;
}

bool Importer::constructModels()
{	
	for (unsigned int i = 0; i < headers.mesh_count; i++)
		numModels += meshHeaders[i].transform_count;

	models = new Model[numModels];
	
	unsigned int modelID = 0;

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		for (unsigned int j = 0; j < meshHeaders[i].transform_count; j++)
		{
			models[modelID].MeshID = i;
			models[modelID].MaterialID = j;

			unsigned int& parent = meshes[i].transform_Id[j];

			memcpy(models[modelID].position, transforms[parent].position, sizeof(double) * 3);
			memcpy(models[modelID].rotation, transforms[parent].rotation, sizeof(double) * 4);
			memcpy(models[modelID].scale, transforms[parent].scale, sizeof(double) * 3);

			modelID++;
		}
	}

	return true;
}

const Material* Importer::getMatrials() const
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
		return meshHeaders[meshID].indice_count;
	else
		return 0;
}