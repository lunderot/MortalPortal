#ifndef COMMONDECLARATION_H
#define COMMONDECLARATION_H
#include <vector>
#include <fstream>

using namespace std;

struct Header
{
	Header()
	{
		group_count = mesh_count = material_count = camera_count = joint_count = light_count = nurb_count = anim_curve_count = skin_count = morph_count = 0;
	};

	unsigned int group_count;
	unsigned int mesh_count;
	unsigned int material_count;
	unsigned int camera_count;
	unsigned int light_count;
	unsigned int joint_count;
	unsigned int nurb_count;
	unsigned int anim_curve_count;
	unsigned int skin_count;
	unsigned int morph_count;

	friend std::ostream& operator<<(std::ostream& out, const Header& obj);
};

struct TransformData
{
	unsigned int name_Length;
	int parentID;
	double position[3];
	double rotation[4];
	double scale[3];

	friend std::ostream& operator<<(std::ostream& out, const TransformData& obj);
};

struct JointData
{
	double jointOrientation[4];
	TransformData transform;

	friend std::ostream& operator<<(std::ostream& out, const JointData& obj)
	{
		out << "ParentID: " << obj.transform.parentID << endl
			<< "Position: " << obj.transform.position[0] << ' ' << obj.transform.position[1] << ' ' << obj.transform.position[3] << endl
			<< "Rotation: " << obj.transform.rotation[0] << ' ' << obj.transform.rotation[1] << ' ' << obj.transform.rotation[2] << ' ' << obj.transform.rotation[3] << endl
			<< "Scale: " << obj.transform.scale[0] << ' ' << obj.transform.scale[1] << ' ' << obj.transform.scale[2] << endl
			<< "Orientation: " << obj.jointOrientation[0] << ' ' << obj.jointOrientation[1] << ' ' << obj.jointOrientation[2] << endl;
		return out;
	}
};


struct Vertex
{
	unsigned int position;
	unsigned int uv;
	unsigned int normal;

	friend std::ostream& operator<<(std::ostream& out, const Vertex& obj)
	{
		out << obj.position <<
			" / " << obj.uv <<
			" / " << obj.normal;
		return out;
	}
};

struct meshStructData
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
	//unsigned int joint_count;
	bool has_Animation;

	//double** position;
	//float** uv;
	//double** normal;
	//double** tangent;
	//double** bi_tangent;

	//int* transform_Id;
	//int* material_Id;
	//Vertex* vertices;
	//const char* name;

	friend std::ostream& operator<<(std::ostream& out, const meshStructData& obj);
};

enum projection_type{ ePerspective, eOrthogonal };
struct cameraData
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

	friend std::ostream& operator<<(std::ostream& out, const cameraData& obj);
};

struct MorphAnimationData
{
	unsigned int blendShape_name_length;

	unsigned int nrOfWeights;
	unsigned int nrOfTargets;
	unsigned int nrOfVertsPerMesh;

	unsigned int meshID;

	unsigned int nrOfPositions;

	friend std::ostream& operator<<(std::ostream& out, const MorphAnimationData& obj);
	// vertex
	// color
};

enum material_type { eLambert, eBlinn, ePhong };
struct MaterialData
{
	MaterialData()
	{
		name_length = duffuse_map_length = normal_map_length = specular_map_length = 0;
		mtrl_type = eLambert;
		normal_depth = specular_factor = shininess = reflection_factor = diffuse_factor = 0;
		specular[3] = reflection[3] = ambient[3] = diffuse[3] = transparency_color[3] = incandescence[3] = { 0.0f };
		//node_name = nullptr;
		//diffuse_map = nullptr;
		//normal_map = nullptr;
		//specular_map = nullptr;
	}

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
	//const char* node_name;
	//char* diffuse_map;
	//char* normal_map;
	//char* specular_map;

	friend std::ostream& operator<<(std::ostream& out, const MaterialData& obj);
};

enum light_type{ ePoint, eDirectional, eSpot, eArea, eVolume };
enum decay_type{ eNone, eLinear, eQuadric, eCubic };
struct LightData
{
	unsigned int name_Length;
	light_type type;
	double color[3];
	float intensity;
	decay_type dType;
	//short decay_type;
	bool cast_shadows;
	double shadow_color[3];
	//const char* name;

	friend std::ostream& operator<<(std::ostream& out, const LightData& obj);

};

struct NurbData
{
	unsigned int name_Length;
	unsigned int numberOfParent;
	float radius;
	//int* parentID;
	//const char* name;

	friend std::ostream& operator<<(std::ostream& out, const NurbData& obj);
};

enum TangentType {
	kTangentGlobal,
	kTangentFixed,
	kTangentLinear,
	kTangentFlat,
	kTangentSmooth,
	kTangentStep,
	kTangentSlow,
	kTangentFast,
	kTangentClamped,
	kTangentPlateau,
	kTangentStepNext
};
// Handles a single point on the Graph Editor's curve. This represents a single keyframe.
struct KeyframePoint
{
	// Time of the point in seconds
	double time;
	// The attribute's value
	double value;
	// The point's in and out tangent positions in X and Y direction.
	float tangentInX;
	float tangentInY;
	float tangentOutX;
	float tangentOutY;
	// Which type of input and output curve the point has.
	TangentType inputTangentType;
	TangentType outputTangentType;

	friend std::ostream& operator<<(std::ostream& out, const KeyframePoint& obj)
	{
		out << "Located at " << obj.time << " seconds" << endl
			<< "Value: " << obj.value << endl
			<< "Input tangent at:  X = " << obj.tangentInX << " Y = " << obj.tangentInY << endl
			<< "Output tangent at: X = " << obj.tangentOutX << " Y = " << obj.tangentOutY << endl
			<< "Input tangent type:  " << obj.inputTangentType << endl
			<< "Output tangent type: " << obj.outputTangentType << endl;

		return out;
	}
};

enum AffectedType { kJoint, kTransform, kBlendShape, kOther };
struct KeyframesData
{
	unsigned int curveNameLength;
	// Whether the animation should loop indefinitely.
	bool loopAnimation;
	// What type of object the keyframes are linked to.
	AffectedType affectedObjectType;
	// Which index the linked object has.
	unsigned int affectedObjectIndex;
	unsigned int numberOfKeyframes;
	unsigned int attachToNameLength;
	//KeyframePoint* points;
	//const char* curveName;
	// Which attribute to attach to. Needs to be non-const because *Maya*.
	//char* attachToName;

	//void WriteBinary(ofstream& outputfile);
	friend std::ostream& operator<<(std::ostream& out, const KeyframesData& obj);
};

// Holds the four influence objects' indices and their weights for a single vertex.
// The first index is paired with the first weight. The second with the second, and so on. 
// An index of -1 should be ignored when parsed.
struct VertexInfluence
{
	// The index of the influence object.
	int influenceObject[4];
	// The weight of the respective index. Is normalized.
	double weight[4];

	friend std::ostream& operator<<(std::ostream& out, const VertexInfluence& obj)
	{
		out << "Influence object index " << obj.influenceObject[0] << " weight: " << obj.weight[0] << endl
			<< "Influence object index " << obj.influenceObject[1] << " weight: " << obj.weight[1] << endl
			<< "Influence object index " << obj.influenceObject[2] << " weight: " << obj.weight[2] << endl
			<< "Influence object index " << obj.influenceObject[3] << " weight: " << obj.weight[3] << endl;

		return out;
	}
};

struct SkinAnimation
{
	// The number of influence objects (e.g. the number of joints).
	unsigned int numberOfInfluences;
	// The index of the mesh to influence.
	unsigned int skinMeshIndex;
	// The number of vertices (and therefore weights) on the mesh.
	unsigned int skinVertexCount;

	// The indices for the influence objects (e.g. the indices for the joints).
	//int* influenceIndices;
	// The data of weights for every vertex. Are as many as there are verts on the mesh.
	//VertexInfluence* influenceWeights;

	friend std::ostream& operator<<(std::ostream& out, const SkinAnimation& obj);
};

#endif // COMMONDECLARATION_H