#include "CommonDeclaration.h"

//Header

void Header::WriteBinary(ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write(output, sizeof(Header));
}

std::ostream& operator<<(std::ostream& out, const Header& obj)
{
	out << "Number of groups: " << obj.group_count << endl
		<< "Number of meshes: " << obj.mesh_count << endl
		<< "Number of materials: " << obj.material_count << endl
		<< "Number of cameras: " << obj.camera_count << endl
		<< "Number of lights: " << obj.light_count << endl
		<< "Number of joints: " << obj.joint_count << endl
		<< "Number of nurbSurfices: " << obj.nurb_count << endl;
	return out;
}

//Transform
void Transform::WriteBinary(ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write(output, sizeof(Transform) - sizeof(const char*));
	outputfile.write(name, sizeof(const char) * name_Length);
}

std::ostream& operator<<(std::ostream& out, const Transform& obj)
{
	out << "Transform name length: " << obj.name_Length << endl
		<< "ParentID: " << obj.parentID << endl
		<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
		<< "Rotation: " << obj.rotation[0] << ' ' << obj.rotation[1] << ' ' << obj.rotation[2] << ' ' << obj.rotation[3] << endl
		<< "Scale: " << obj.scale[0] << ' ' << obj.scale[1] << ' ' << obj.scale[2] << endl
		<< "Transform Name: " << obj.name << endl;
	return out;
}

//Camera
void camera::WriteBinary(ofstream& outputfile)
{
	//MGlobal::displayInfo(MString() + header->name_length);
	char* output = (char*) this;
	outputfile.write((const char*)output, sizeof(camera) - sizeof(const char*) - sizeof(unsigned int*));
	outputfile.write((const char*)parentID, sizeof(int) * nrOfParents);
	outputfile.write(name, name_length);
}

std::ostream& operator<<(std::ostream& out, const camera& obj)
{
	out << "Camera name length: " << obj.name_length << endl
		<< "Number of parents: " << obj.nrOfParents << endl;
	out << "CAMERA" << endl;
	out << "ParentID: " << obj.parentID[0] << endl;
	out << "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
		<< "Up Vector: " << obj.up_vector[0] << ' ' << obj.up_vector[1] << ' ' << obj.up_vector[2] << endl
		<< "Intrest position: " << obj.interest_position[0] << ' ' << obj.interest_position[1] << ' ' << obj.interest_position[2] << endl
		<< "FOV X: " << obj.field_of_view_x << endl
		<< "FOV Y: " << obj.field_of_view_y << endl
		<< "Near Plane: " << obj.near_plane << endl
		<< "Far Plane: " << obj.far_plane << endl
		<< "Projection type: " << obj.projection << endl
		<< "Name: " << obj.name << endl;
	return out;
}

//Mateial
void MaterialData::WriteBinary(ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write((const char*)output, sizeof(MaterialData) - sizeof(const char*) * 4);
	outputfile.write(node_name, sizeof(char) * name_length);
	outputfile.write(diffuse_map, sizeof(char) * duffuse_map_length);
	outputfile.write(normal_map, sizeof(char) * normal_map_length);
	outputfile.write(specular_map, sizeof(char) * specular_map_length);
}

std::ostream& operator<<(std::ostream& out, const MaterialData& obj)
{
	out << "Material type: " << obj.mtrl_type << endl
		<< "Normal depth: " << obj.normal_depth << endl
		<< "Specular: " << obj.specular[0] << " " << obj.specular[1] << " " << obj.specular[2] << endl
		<< "Specular factor: " << obj.specular_factor << endl
		<< "Shininess: " << obj.shininess << endl
		<< "Reflection: " << obj.reflection[0] << " " << obj.reflection[1] << " " << obj.reflection[2] << endl
		<< "Reflection factor: " << obj.reflection_factor << endl
		<< "Ambient: " << obj.ambient[0] << " " << obj.ambient[1] << " " << obj.ambient[2] << endl
		<< "Diffuse: " << obj.diffuse[0] << " " << obj.diffuse[1] << " " << obj.diffuse[2] << endl
		<< "Diffuse factor: " << obj.diffuse_factor << endl
		<< "Transparency color: " << obj.transparency_color[0] << " " << obj.transparency_color[1] << " " << obj.transparency_color[2] << endl
		<< "Incandescence: " << obj.incandescence[0] << " " << obj.incandescence[1] << " " << obj.incandescence[2] << endl
		<< "Name: " << obj.node_name << endl
		<< "Diffuse map: ";
	if (obj.diffuse_map != nullptr)
		out << obj.diffuse_map;
	out << endl
		<< "Normal map: ";
	if (obj.diffuse_map != nullptr)
		out << obj.normal_map;
	out << endl
		<< "Specular map: ";
	if (obj.diffuse_map != nullptr)
		out << obj.specular_map;
	out << endl;
	return out;
}

// Light 
void Light::WriteBinary(ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write(output, sizeof(Light) - sizeof(const char*));
	outputfile.write(name, name_Length);
}

std::ostream& operator<<(std::ostream& out, const Light& obj)
{
	out << "LIGHT" << endl
		<< "Light name length: " << obj.name_Length << endl
		<< "Name of light: " << obj.name << endl
		<< "Light type: " << obj.type << endl
		<< "Decay type: " << obj.dType << endl
		<< "Color: " << obj.color[0] << " " << obj.color[1] << " " << obj.color[2] << endl
		<< "Intensity: " << obj.intensity << endl
		<< "Cast shadow: " << obj.cast_shadows << endl
		<< "Shadow color: " << obj.shadow_color[0] << " " << obj.shadow_color[1] << " " << obj.shadow_color[2] << endl;
	return out;
}

// Nurb
void Nurb::WriteBinary(ofstream& outputfile)
{

	char* output = (char*) this;
	outputfile.write(output, sizeof(Nurb) - sizeof(const char*) - sizeof(int*));
	outputfile.write((const char*)parentID, sizeof(int) * numberOfParent);
	outputfile.write(name, name_Length);
}

// Nurb
std::ostream& operator << (std::ostream& out, const Nurb& obj)
{
	out << "NURB" << endl
		<< " Nurb name lengtht: " << obj.name_Length << endl
		<< " Number of nurbs: " << obj.numberOfParent << endl
		<< "Name of nurb: " << obj.name << endl
		<< "Radius: " << obj.radius << endl
		<< "Parent ID: " << obj.parentID[0] << endl;

	return out;

}

//struct MorphAnimation

void MorphAnimation::WriteBinary(ofstream& outputfile)
{
	//char* output = (char*) this;
	//outputfile.write((const char*)output, sizeof(meshStruct) - sizeof(const char*));
	for (unsigned int i = 0; i < nrOfVertsPerMesh * nrOfWeights; i++)
		outputfile.write((char*)position[i].data(), sizeof(double) * 3);

	outputfile.write(blendShapeName, blendShape_name_length);
}

std::ostream& operator<<(std::ostream& out, const MorphAnimation& obj)
{
	out << "Blend Shape name length: " << obj.blendShape_name_length << endl;

	out << "nrOfWeights: " << obj.nrOfWeights << endl
		<< "nrOfTargets: " << obj.nrOfTargets << endl
		<< "nrOfVertsPerWeight: " << obj.nrOfVertsPerMesh << endl;

	for (unsigned int i = 0; i < obj.position.size(); i++)
	{
		out << "Position:" << obj.position[i][0] << "/" << obj.position[i][1] << "/" << obj.position[i][2] << endl;
	}
	out << "blendShapeName" << obj.blendShapeName << endl
		<< "Mesh ID: " << obj.meshID << endl;

	return out;
}

//meshStruct
void meshStruct::WriteBinary(ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write((const char*)output, sizeof(meshStruct) - sizeof(const char*) * 9 - 3);

	for (unsigned int i = 0; i < position_count; i++)
		outputfile.write((char*)position[i], sizeof(double) * 3);

	for (unsigned int i = 0; i < uv_count; i++)
		outputfile.write((char*)uv[i], sizeof(float) * 2);

	for (unsigned int i = 0; i < normal_count; i++)
		outputfile.write((char*)normal[i], sizeof(double) * 3);

	for (unsigned int i = 0; i < tangent_count; i++)
		outputfile.write((char*)tangent[i], sizeof(double) * 3);

	for (unsigned int i = 0; i < biTangent_count; i++)
		outputfile.write((char*)bi_tangent[i], sizeof(double) * 3);

	outputfile.write((char*)transform_Id, transform_count * sizeof(int));

	outputfile.write((char*)material_Id, material_count * sizeof(int));

	outputfile.write((char*)vertices, indice_count * sizeof(Vertex));

	outputfile.write(name, name_length);
}

std::ostream& operator<<(std::ostream& out, const meshStruct& obj)
{
	out << "Mesh Name: " << obj.name << endl
		<< "Name Length: " << obj.name_length << endl
		<< "Vertex Count: " << obj.vertex_count << endl
		<< "Indices Count: " << obj.indice_count << endl
		<< "Position Count: " << obj.position_count << endl
		<< "Uv Count: " << obj.uv_count << endl
		<< "Normal Count: " << obj.normal_count << endl
		<< "tangent Count: " << obj.tangent_count << endl
		<< "bi-Tangent Count: " << obj.biTangent_count << endl
		<< "Material Count: " << obj.material_count << endl
		<< "Transform Count: " << obj.transform_count << endl
		<< "Transform id: " << endl;

	for (unsigned int i = 0; i < obj.transform_count; i++)
	{
		out << obj.transform_Id[i] << endl;
	}

	out << "Material id: " << endl;

	for (unsigned int i = 0; i < obj.material_count; i++)
	{
		out << obj.material_Id[i] << endl;
	}

	for (unsigned int i = 0; i < obj.position_count; i++)
	{
		out << "Position " << i << ": " << obj.position[i][0] << " " << obj.position[i][1] << " " << obj.position[i][2] << endl;
	}
	for (unsigned int i = 0; i < obj.uv_count; i++)
	{
		out << "UV " << i << ": " << obj.uv[i][0] << " " << obj.uv[i][1] << endl;
	}
	for (unsigned int i = 0; i < obj.normal_count; i++)
	{
		out << "Normal " << i << ": " << obj.normal[i][0] << " " << obj.normal[i][1] << " " << obj.normal[i][2] << endl;
	}
	for (unsigned int i = 0; i < obj.tangent_count; i++)
	{
		out << "Tangent " << i << ": " << obj.tangent[i][0] << " " << obj.tangent[i][1] << " " << obj.tangent[i][2] << endl;
	}
	for (unsigned int i = 0; i < obj.biTangent_count; i++)
	{
		out << "Bi-Tangent " << i << ": " << obj.bi_tangent[i][0] << " " << obj.bi_tangent[i][1] << " " << obj.bi_tangent[i][2] << endl;
	}

	int tmp = 0;
	int tmp_faceCounter = 0;
	for (unsigned int i = 0; i < obj.indice_count; i++)
	{
		if (tmp == 0 || tmp == 3)
		{
			out << "Face " << tmp_faceCounter << ": " << endl;
			tmp_faceCounter++;
			tmp = 0;
		}
		out << obj.vertices[i] << endl;
		tmp++;
	}

	return out;
}