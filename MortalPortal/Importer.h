#ifndef IMPORTER_H
#define IMPORTER_H
#include "CommonDeclaration.h"
#include <string>

struct ImporterTransform
{
	unsigned int name_Length;
	int parentID;
	double position[3];
	double rotation[4];
	double scale[3];
	char*  name;
};

struct ImporterJoint
{
	double jointOrientation[4];
	ImporterTransform transform;
};

struct ImporterMesh
{
	unsigned int name_length;
	unsigned int vertex_count;
	unsigned int indice_count;
	unsigned int position_count;
	unsigned int uv_count;
	unsigned int normal_count;
	unsigned int tangent_count;
	unsigned int biTangent_count;
	unsigned int material_count;
	unsigned int transform_count;
	bool has_Animation;

	double* position;
	float* uv;
	double* normal;
	double* tangent;
	double* bi_tangent;

	int* transform_Id;
	int* material_Id;
	Vertex* vertices;
	char*  name;
};

struct ImporterCamera 
{
	unsigned int name_length;
	unsigned int nrOfParents;
	double position[3];
	double up_vector[3];
	double interest_position[3];
	double field_of_view_x;
	double field_of_view_y;
	double near_plane;
	double far_plane;
	projection_type projection;
	unsigned int* parentID;
	char*  name;
};

struct ImporterMaterial
{
	unsigned int name_length;
	unsigned int duffuse_map_length;
	unsigned int normal_map_length;
	unsigned int specular_map_length;
	material_type mtrl_type;
	double normal_depth;
	double specular[3];
	double specular_factor;
	double shininess;
	double reflection[3];
	double reflection_factor;
	double ambient[3];
	double diffuse[3];
	double diffuse_factor;
	double transparency_color[3];
	double incandescence[3];
	char* name;
	char* diffuse_map;
	char* normal_map;
	char* specular_map;
};

struct ImporterLight
{
	unsigned int name_Length;
	unsigned int numberOfParents;
	light_type type;
	double color[3];
	float intensity;
	decay_type dType;
	bool cast_shadows;
	double shadow_color[3];
	int* parentID;
	char* name;
};

struct ImporterNurb
{
	unsigned int name_Length;
	unsigned int numberOfParent;
	float radius;
	int* ParentID;
	char* name;
};

struct ImporterMorphAnimation
{
	unsigned int blendShape_name_length;
	unsigned int nrOfWeights;
	unsigned int nrOfTargets;
	unsigned int nrOfVertsPerMesh;
	unsigned int meshID;
	unsigned int nrOfPositions;

	double* positions;
	char* name;
};

struct ImporterWeights
{
	unsigned int numberOfInfluences;
	unsigned int skinMeshIndex;
	unsigned int skinVertexCount;
	int * influenceIndices;
	VertexInfluence* Weights;
};

struct ImporterKeyframes
{
	unsigned int curveNameLength;
	bool loopAnimation;
	AffectedType affectedObjectType;
	unsigned int affectedObjectIndex;
	unsigned int numberOfKeyframes;
	unsigned int attachToNameLength;
	KeyframePoint* keys;
	char* curvaName;
	char* attachToName;
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
	unsigned int getNumMorph() const;
	unsigned int getNumWeights() const;
	unsigned int getNumCurves() const;

	const ImporterTransform* getTransform() const;
	const ImporterMesh* getMesh() const;
	const ImporterCamera* getCamera() const;
	const ImporterMaterial* getMaterial() const;
	const ImporterLight* getLight() const;
	const ImporterJoint* getJoint() const;
	const ImporterNurb* getNurb() const;
	const ImporterMorphAnimation* getMorph() const;
	const ImporterWeights* getWeights() const;
	const ImporterKeyframes* getCurves() const;

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
	bool extractMorphs(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractSkinAnimations(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractKeys(unsigned int& offset, char* fileData, unsigned int& fileSize);

	Header headers;

	ImporterTransform* transforms;
	ImporterMesh* meshes;
	ImporterCamera* cameras;
	ImporterMaterial* materials;
	ImporterLight* lights;
	ImporterJoint* joints;
	ImporterNurb* nurbs;
	ImporterMorphAnimation* morph;
	ImporterWeights* skins;
	ImporterKeyframes* Curves;
};

#endif