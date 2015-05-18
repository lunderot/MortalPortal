#include "Importer.h"
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

Importer::Importer()
{
	transforms = nullptr;
	meshes = nullptr;
	cameras = nullptr;
	materials = nullptr;
	lights = nullptr;
	//nurbs = nullptr;
};

Importer::~Importer()
{
	for (unsigned int i = 0; i < headers.group_count; i++)
		delete[] transforms[i].name;

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{ 
		delete[] meshes[i].position;
		delete[] meshes[i].uv;
		delete[] meshes[i].normal;
		delete[] meshes[i].tangent;
		delete[] meshes[i].bi_tangent;
		delete[] meshes[i].transform_Id;
		delete[] meshes[i].material_Id;
		delete[] meshes[i].vertices;
		delete[] meshes[i].name;
	}

	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		delete[] cameras[i].parentID;
		delete[] cameras[i].name;
	}

	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		delete[] materials[i].name;
		if (materials[i].diffuse_map != nullptr)
			delete[] materials[i].diffuse_map;
		if (materials[i].normal_map != nullptr)
			delete[] materials[i].normal_map;
		if (materials[i].specular_map != nullptr)
			delete[] materials[i].specular_map;
	}

	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		delete[] lights[i].parentID;
		delete[] lights[i].name;		
	}

	for (unsigned int i = 0; i < headers.joint_count; i++)
		delete[] joints[i].transform.name;

	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		delete[] nurbs[i].ParentID;
		delete[] nurbs[i].name;
	}

	for (unsigned int i = 0; i < headers.morph_count; i++)
	{
		delete[] morph[i].positions;
		delete[] morph[i].name;
	}

	for (unsigned int i = 0; i < headers.skin_count; i++)
	{
		delete[] skins[i].influenceIndices;
		delete[] skins[i].Weights;
	}

	for (unsigned int i = 0; i < headers.anim_curve_count; i++)
	{
		delete[] Curves[i].keys;
		delete[] Curves[i].curvaName;
		delete[] Curves[i].attachToName;
	}

	delete[] transforms;
	delete[] meshes;
	delete[] cameras;
	delete[] materials;
	delete[] lights;
	delete[] joints;
	delete[] nurbs;
	delete[] morph;
	delete[] skins;
	delete[] Curves;
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

	if (!extractLights(offset, fileData, fileByteSize)) return false;

	if (!extractNurb(offset, fileData, fileByteSize)) return false;

	if (!extractMeshes(offset, fileData, fileByteSize)) return false;

	if (!extractMorphs(offset, fileData, fileByteSize)) return false;

	if (!extractSkinAnimations(offset, fileData, fileByteSize)) return false;

	if (!extractKeys(offset, fileData, fileByteSize)) return false;

	delete fileData;

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
	transforms = new ImporterTransform[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		if (sizeof(TransformData) > fileSize - offset)
			return false;

		memcpy(&transforms[i], &fileData[offset], sizeof(TransformData));
		offset += sizeof(TransformData);

		if (transforms[i].name_Length > fileSize - offset)
			return false;

		char* node_name = new char[transforms[i].name_Length + 1];
		node_name[transforms[i].name_Length] = '\0';
	
		memcpy((char*)node_name, &fileData[offset], transforms[i].name_Length);
		transforms[i].name = node_name;

		offset += transforms[i].name_Length;	
	}

	return true;
}

bool Importer::extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	meshes = new ImporterMesh[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		if (sizeof(meshStructData) > fileSize - offset)
			return false;
		memcpy(&meshes[i], &fileData[offset], sizeof(meshStructData));
		offset += sizeof(meshStructData);

		if (meshes[i].position_count * 3 * sizeof(double) > fileSize - offset)
			return false;
		meshes[i].position = new double[meshes[i].position_count * 3];
		memcpy(meshes[i].position, &fileData[offset], meshes[i].position_count * 3 * sizeof(double));
		offset += meshes[i].position_count * 3 * sizeof(double);

		if (meshes[i].uv_count * 2 * sizeof(float) > fileSize - offset)
			return false;
		meshes[i].uv = new float[meshes[i].uv_count * 2];
		memcpy(meshes[i].uv, &fileData[offset], meshes[i].uv_count * 2 * sizeof(float));
		offset += meshes[i].uv_count * 2 * sizeof(float);

		if (meshes[i].normal_count * 3 * sizeof(double) > fileSize - offset)
			return false;
		meshes[i].normal = new double[meshes[i].normal_count * 3];
		memcpy(meshes[i].normal, &fileData[offset], meshes[i].normal_count * 3 * sizeof(double));
		offset += meshes[i].normal_count * 3 * sizeof(double);

		if (meshes[i].tangent_count * 3 * sizeof(double) > fileSize - offset)
			return false;
		meshes[i].tangent = new double[meshes[i].tangent_count * 3];
		memcpy(meshes[i].tangent, &fileData[offset], meshes[i].tangent_count * 3 * sizeof(double));
		offset += meshes[i].tangent_count * 3 * sizeof(double);

		if (meshes[i].biTangent_count * 3 * sizeof(double) > fileSize - offset)
			return false;
		meshes[i].bi_tangent = new double[meshes[i].biTangent_count * 3];
		memcpy(meshes[i].bi_tangent, &fileData[offset], meshes[i].biTangent_count * 3 * sizeof(double));
		offset += meshes[i].biTangent_count * 3 * sizeof(double);

		meshes[i].vertices = new Vertex[meshes[i].indice_count];
		meshes[i].material_Id = new int[meshes[i].material_count];
		meshes[i].transform_Id = new int[meshes[i].transform_count];
		char* name = new char[meshes[i].name_length + 1];
		name[meshes[i].name_length] = '\0';

		if (meshes[i].transform_count * sizeof(int) > fileSize - offset)
			return false;
		memcpy(meshes[i].transform_Id, &fileData[offset], meshes[i].transform_count * sizeof(int));
		offset += meshes[i].transform_count * sizeof(int);

		if (meshes[i].material_count * sizeof(int) > fileSize - offset)
			return false;
		memcpy(meshes[i].material_Id, &fileData[offset], meshes[i].material_count * sizeof(int));
		offset += meshes[i].material_count * sizeof(int);

		if (meshes[i].indice_count * sizeof(Vertex) > fileSize - offset)
			return false;
		memcpy(meshes[i].vertices, &fileData[offset], meshes[i].indice_count * sizeof(Vertex));
		offset += meshes[i].indice_count * sizeof(Vertex);

		memcpy(name, &fileData[offset], meshes[i].name_length);
		meshes[i].name = name;
		offset += meshes[i].name_length;

	}

	return true;
}

bool Importer::extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	cameras = new ImporterCamera[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		if (sizeof(cameraData) > fileSize - offset)
			return false;
		memcpy(&cameras[i], &fileData[offset], sizeof(cameraData));
		offset += sizeof(cameraData);

		if (sizeof(unsigned int) * cameras[i].nrOfParents > fileSize - offset)
			return false;
		cameras[i].parentID = new unsigned int[cameras[i].nrOfParents];
		memcpy(cameras[i].parentID, &fileData[offset], sizeof(int) * cameras[i].nrOfParents);
		offset += sizeof(unsigned int) * cameras[i].nrOfParents;

		if (cameras[i].name_length > fileSize - offset)
			return false;
		char* name = new char[cameras[i].name_length + 1];
		name[cameras[i].name_length] = '\0';
		
		memcpy(name, &fileData[offset], cameras[i].name_length);

		cameras[i].name = name;

		offset += cameras[i].name_length;
	}

	return true;
}

bool Importer::extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	materials = new ImporterMaterial[headers.material_count];
	
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		if (sizeof(MaterialData) > fileSize - offset)
			return false;
		memcpy(&materials[i], &fileData[offset], sizeof(MaterialData));
		offset += sizeof(MaterialData);

		if (materials[i].name_length > fileSize - offset)
			return false;
		char* name = new char[materials[i].name_length + 1];
		name[materials[i].name_length] = '\0';

		memcpy(name, &fileData[offset], materials[i].name_length);
		offset += materials[i].name_length;
		materials[i].name = name;

	/*	memcpy(materials[i].ambient, materials[i].ambient, sizeof(matData.ambient));
		memcpy(materials[i].diffuse, materials[i].diffuse, sizeof(matData.diffuse));
		memcpy(materials[i].incandescence, materials[i].incandescence, sizeof(matData.incandescence));
		memcpy(materials[i].reflection, materials[i].reflection, sizeof(matData.reflection));
		memcpy(materials[i].specular, matData.specular, sizeof(matData.specular));
		memcpy(materials[i].transparency_color, matData.transparency_color, sizeof(matData.transparency_color));

		materials[i].mtrl_type = matData.mtrl_type;	
		materials[i].diffuse_factor = matData.diffuse_factor;
		materials[i].shininess = matData.shininess;
		materials[i].normal_depth = matData.normal_depth;
		materials[i].reflection_factor = matData.reflection_factor;
		materials[i].specular_factor = matData.specular_factor;*/

		if (materials[i].duffuse_map_length != 0)
		{
			if (materials[i].duffuse_map_length > fileSize - offset)
				return false;

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
			if (materials[i].normal_map_length > fileSize - offset)
				return false;

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
			if (materials[i].specular_map_length > fileSize - offset)
				return false;

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
	lights = new ImporterLight[headers.light_count];
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		if (sizeof(LightData) > fileSize - offset)
			return false;
		memcpy(&lights[i], &fileData[offset], sizeof(LightData));
		offset += sizeof(LightData);

		if (lights[i].numberOfParents * sizeof(int) > fileSize - offset)
			return false;
		lights[i].parentID = new int[lights[i].numberOfParents];
		memcpy(lights[i].parentID, &fileData[offset], lights[i].numberOfParents * sizeof(int));

		if (lights[i].name_Length > fileSize - offset)
			return false;
		char* name = new char[lights[i].name_Length + 1];
		name[lights[i].name_Length] = '\0';
		
		memcpy(name, &fileData[offset], lights[i].name_Length);
		lights[i].name = name;

		offset += lights[i].name_Length;
	}

	return true;
}

bool Importer::extractJoint(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	joints = new ImporterJoint[headers.joint_count];
	for (unsigned int i = 0; i < headers.joint_count; i++)
	{
		if (sizeof(JointData)> fileSize - offset)
			return false;
		memcpy(&joints[i], &fileData[offset], sizeof(JointData));
		offset += sizeof(JointData);

		if (joints[i].transform.name_Length > fileSize - offset)
			return false;
		char* name = new char[joints[i].transform.name_Length + 1];
		name[joints[i].transform.name_Length] = '\0';	

		memcpy(name, &fileData[offset], joints[i].transform.name_Length);
		joints[i].transform.name = name;
		offset += joints[i].transform.name_Length;
	}

	return true;
}

bool Importer::extractNurb(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	nurbs = new ImporterNurb[headers.nurb_count];
	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		if (sizeof(NurbData) > fileSize - offset)
			return false;
		memcpy(&nurbs[i], &fileData[offset], sizeof(NurbData));
		offset += sizeof(NurbData);

		if (nurbs[i].numberOfParent * sizeof(int)> fileSize - offset)
			return false;
		nurbs[i].ParentID = new int[nurbs[i].numberOfParent];
		memcpy(nurbs[i].ParentID, &fileData[offset], nurbs[i].numberOfParent * sizeof(int));
		offset += nurbs[i].numberOfParent * sizeof(int);

		if (nurbs[i].name_Length > fileSize - offset)
			return false;
		char* name = new char[nurbs[i].name_Length + 1];
		name[nurbs[i].name_Length] = '\0';
		
		memcpy(name, &fileData[offset], nurbs[i].name_Length);
		nurbs[i].name = name;

		offset += nurbs[i].name_Length;
	}

	return true;
}

bool Importer::extractMorphs(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	morph = new ImporterMorphAnimation[headers.morph_count];
	for (unsigned int i = 0; i < headers.morph_count; i++)
	{
		if (sizeof(MorphAnimationData) > fileSize - offset)
			return false;
		memcpy(&morph[i], &fileData[offset], sizeof(MorphAnimationData));
		offset += sizeof(MorphAnimationData);

		if (morph[i].nrOfPositions * 3 * sizeof(double)> fileSize - offset)
			return false;
		morph[i].positions = new double[morph[i].nrOfPositions * 3];
		memcpy((char*)morph[i].positions, &fileData[offset], morph[i].nrOfPositions * 3 * sizeof(double));
		offset += morph[i].nrOfPositions * 3 * sizeof(double);

		if (morph[i].blendShape_name_length > fileSize - offset)
			return false;
		char* name = new char[morph[i].blendShape_name_length + 1];
		name[morph[i].blendShape_name_length] = '\0';
		memcpy(name, &fileData[offset], morph[i].blendShape_name_length);
		morph[i].name = name;

		offset += morph[i].blendShape_name_length;
	}
	return true;
}

bool Importer::extractSkinAnimations(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	skins = new ImporterWeights[headers.skin_count];
	for (unsigned int i = 0; i < headers.skin_count; i++)
	{
		if (sizeof(SkinAnimation)> fileSize - offset)
			return false;
		memcpy(&skins[i], &fileData[offset], sizeof(SkinAnimation));
		offset += sizeof(SkinAnimation);
		
		if (skins[i].numberOfInfluences * sizeof(int) > fileSize - offset)
			return false;
		skins[i].influenceIndices = new int[skins[i].numberOfInfluences];
		memcpy(&skins[i].influenceIndices, &fileData[offset], skins[i].numberOfInfluences * sizeof(int));
		offset += skins[i].numberOfInfluences * sizeof(int);

		if (skins[i].skinVertexCount * sizeof(VertexInfluence) > fileSize - offset)
			return false;
		skins[i].Weights = new VertexInfluence[skins[i].skinVertexCount];
		memcpy(&skins[i].Weights, &fileData[offset], skins[i].skinVertexCount * sizeof(VertexInfluence));
		offset += skins[i].skinVertexCount * sizeof(VertexInfluence);
	}
	return true;
}

bool Importer::extractKeys(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	Curves = new ImporterKeyframes[headers.anim_curve_count];
	for (unsigned int i = 0; i < headers.anim_curve_count; i++)
	{
		if (sizeof(KeyframesData) > fileSize - offset)
			return false;
		memcpy(&Curves[i], &fileData[offset], sizeof(KeyframesData));
		offset += sizeof(KeyframesData);

		if (Curves[i].numberOfKeyframes * sizeof(KeyframePoint) > fileSize - offset)
			return false;
		Curves[i].keys = new KeyframePoint[Curves[i].numberOfKeyframes];
		memcpy(&Curves[i].keys, &fileData[offset], Curves[i].numberOfKeyframes * sizeof(KeyframePoint));
		offset += Curves[i].numberOfKeyframes * sizeof(KeyframePoint);

		if (Curves[i].curveNameLength > fileSize - offset)
			return false;
		char* name = new char[Curves[i].curveNameLength + 1];
		name[Curves[i].curveNameLength] = '\0';
		memcpy(name, &fileData[offset], Curves[i].curveNameLength);
		Curves[i].curvaName = name;
		offset += Curves[i].curveNameLength;


		if (Curves[i].attachToNameLength > fileSize - offset)
			return false;
		Curves[i].attachToName = new char[Curves[i].attachToNameLength + 1];
		Curves[i].attachToName[Curves[i].attachToNameLength] = '\0';
		memcpy(Curves[i].attachToName, &fileData[offset], Curves[i].attachToNameLength);
		offset += Curves[i].attachToNameLength;
	}
	return true;
}

unsigned int Importer::getNumTransforms() const
{
	return headers.group_count;
}

unsigned int Importer::getNumMaterials() const
{
	return headers.material_count;
}

unsigned int Importer::getNumCameras() const
{
	return headers.camera_count;
}

unsigned int Importer::getNumLights() const
{
	return headers.light_count;
}

unsigned int Importer::getNumMeshes() const
{
	return headers.mesh_count;
}

unsigned int Importer::getNumNurbs() const
{
	return headers.nurb_count;
}

unsigned int Importer::getNumMorph() const
{
	return headers.morph_count;
}

unsigned int Importer::getNumWeights() const
{
	return headers.skin_count;
}

unsigned int Importer::getNumCurves() const
{
	return headers.anim_curve_count;
}

const ImporterTransform* Importer::getTransform() const
{
	return transforms;
}

const ImporterMesh* Importer::getMesh() const
{
	return meshes;
}

const ImporterCamera* Importer::getCamera() const
{
	return cameras;
}

const ImporterMaterial* Importer::getMaterial() const
{
	return materials;
}

const ImporterLight* Importer::getLight() const
{
	return lights;
}

const ImporterJoint* Importer::getJoint() const
{
	return joints;
}

const ImporterNurb* Importer::getNurb() const
{
	return nurbs;
}

const ImporterMorphAnimation* Importer::getMorph() const
{
	return morph;
}

const ImporterWeights* Importer::getWeights() const
{
	return skins;
}

const ImporterKeyframes* Importer::getCurves() const
{
	return Curves;
}
