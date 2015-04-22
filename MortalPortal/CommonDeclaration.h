#ifndef COMMONDECLARATION_H
#define COMMONDECLARATION_H
#include <vector>
#include <fstream>

using namespace std;

struct Header
{
	Header()
	{
		group_count = mesh_count = material_count = camera_count = joint_count = light_count = nurb_count = 0;
	};

	unsigned int group_count;
	unsigned int mesh_count;
	unsigned int material_count;
	unsigned int camera_count;
	unsigned int light_count;
	unsigned int joint_count;
	unsigned int nurb_count;

	void WriteBinary(ofstream& outputfile);
	friend std::ostream& operator<<(std::ostream& out, const Header& obj);
};

struct Transform
{
#ifndef MAYA_EXPORT
	~Transform()
	{
		delete[] name;
	};
#endif
	unsigned int name_Length;
	int parentID;
	double position[3];
	double rotation[4];
	double scale[3];
	const char* name;

	void WriteBinary(ofstream& outputfile);

	friend std::ostream& operator<<(std::ostream& out, const Transform& obj);
};

struct Joint
{
	double jointOrientation[4];
	Transform transform;

	void WriteBinary(ofstream& outputfile)
	{
		char* output = (char*) this;
		outputfile.write((const char*)output, sizeof(double) * 4);
		transform.WriteBinary(outputfile);
	}

	friend std::ostream& operator<<(std::ostream& out, const Joint& obj)
	{
		out << "ParentID: " << obj.transform.parentID << endl
			<< "Position: " << obj.transform.position[0] << ' ' << obj.transform.position[1] << ' ' << obj.transform.position[3] << endl
			<< "Rotation: " << obj.transform.rotation[0] << ' ' << obj.transform.rotation[1] << ' ' << obj.transform.rotation[2] << ' ' << obj.transform.rotation[3] << endl
			<< "Scale: " << obj.transform.scale[0] << ' ' << obj.transform.scale[1] << ' ' << obj.transform.scale[2] << endl
			<< "Orientation: " << obj.jointOrientation[0] << ' ' << obj.jointOrientation[1] << ' ' << obj.jointOrientation[2] << endl
			<< "Joint Name: " << obj.transform.name << endl;
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

struct meshStruct
{
#ifndef MAYA_EXPORT
	~meshStruct()
	{
		delete[] name;
		delete[] vertices;
		delete[] material_Id;
		delete[] transform_Id;

		for (unsigned int i = 0; i < position_count; i++)
		{
			delete[] position[i];
		}
		delete[] position;

		for (unsigned int i = 0; i < uv_count; i++)
		{
			delete[] uv[i];
		}
		delete[] uv;

		for (unsigned int i = 0; i < normal_count; i++)
		{
			delete[] normal[i];
		}
		delete[] normal;

		for (unsigned int i = 0; i < tangent_count; i++)
		{
			delete[] tangent[i];
		}
		delete[] tangent;

		for (unsigned int i = 0; i < biTangent_count; i++)
		{
			delete[] bi_tangent[i];
		}
		delete[] bi_tangent;
	}
#endif
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

	double** position;
	float** uv;
	double** normal;
	double** tangent;
	double** bi_tangent;

	int* transform_Id;
	int* material_Id;
	Vertex* vertices;
	const char* name;

	void WriteBinary(ofstream& outputfile);

	friend std::ostream& operator<<(std::ostream& out, const meshStruct& obj);
};

struct camera
{
#ifndef MAYA_EXPORT
	~camera()
	{
		delete[] parentID;
		delete[] name;
	};
#endif
	unsigned int name_length;
	unsigned int nrOfParents;
	double position[3];
	double up_vector[3];
	double interest_position[3];
	double field_of_view_x;
	double field_of_view_y;
	double near_plane;
	double far_plane;
	enum projection_type{ ePerspective, eOrthogonal } projection;
	unsigned int* parentID;
	const char* name;

	void WriteBinary(ofstream& outputfile);

	friend std::ostream& operator<<(std::ostream& out, const camera& obj);
};

struct MorphAnimation
{
	unsigned int blendShape_name_length;

	unsigned int nrOfWeights;
	unsigned int nrOfTargets;
	unsigned int nrOfVertsPerMesh;

	unsigned int meshID;
	unsigned int nrOfKeys;

	vector <vector<double>> position;
	const char* blendShapeName;

	void WriteBinary(ofstream& outputfile);

	friend std::ostream& operator<<(std::ostream& out, const MorphAnimation& obj);
	// vertex
	// color
};

struct Material
{
	Material()
	{
		name_length = duffuse_map_length = normal_map_length = specular_map_length = 0;
		mtrl_type = eLambert;
		normal_depth = specular_factor = shininess = reflection_factor = diffuse_factor = 0;
		specular[3] = reflection[3] = ambient[3] = diffuse[3] = transparency_color[3] = incandescence[3] = { 0.0f };
		node_name = nullptr;
		diffuse_map = nullptr;
		normal_map = nullptr;
		specular_map = nullptr;
	}
#ifndef MAYA_EXPORT
	~Material()
	{
		delete[] node_name;
		delete[] diffuse_map;
		delete[] normal_map;
		delete[] specular_map;
	};
#endif

	unsigned int name_length;
	unsigned int duffuse_map_length;
	unsigned int normal_map_length;
	unsigned int specular_map_length;
	enum material_type { eLambert, eBlinn, ePhong } mtrl_type;
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
	const char* node_name;
	const char* diffuse_map;
	const char* normal_map;
	const char* specular_map;

	void WriteBinary(ofstream& outputfile);

	friend std::ostream& operator<<(std::ostream& out, const Material& obj);
};

struct Animation
{
	unsigned int joint_ID[4];
	double joint_weight[4];
	// bone-id
	// weight
	// bla bla
};

struct Light
{
	//char name[];
#ifndef MAYA_EXPORT
	~Light()
	{
		delete[] name;
	};
#endif
	unsigned int name_Length;
	enum light_type{ ePoint, eDirectional, eSpot, eArea, eVolume }type;
	double color[3];
	float intensity;
	enum decay_type{ eNone, eLinear, eQuadric, eCubic }dType;
	//short decay_type;
	bool cast_shadows;
	double shadow_color[3];
	const char* name;

	void WriteBinary(ofstream& outputfile);
	friend std::ostream& operator<<(std::ostream& out, const Light& obj);

};

struct Nurb
{
#ifndef MAYA_EXPORT
	~Nurb()
	{
		delete[] parentID;
		delete[] name;
	};
#endif
	unsigned int name_Length;
	unsigned int numberOfParent;
	float radius;
	int* parentID;
	const char* name;

	void WriteBinary(ofstream& outputfile);
	friend std::ostream& operator<<(std::ostream& out, const Nurb& obj);
};

#endif