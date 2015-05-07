#include "CommonDeclaration.h"

//Header
std::ostream& operator<<(std::ostream& out, const Header& obj)
{
	out << "Number of groups: " << obj.group_count << endl
		<< "Number of meshes: " << obj.mesh_count << endl
		<< "Number of materials: " << obj.material_count << endl
		<< "Number of cameras: " << obj.camera_count << endl
		<< "Number of lights: " << obj.light_count << endl
		<< "Number of joints: " << obj.joint_count << endl
		<< "Number of nurbSurfices: " << obj.nurb_count << endl
		<< "Number of animation curves: " << obj.anim_curve_count << endl
		<< "Number of skins: " << obj.skin_count << endl
		<< "Number of morph animations: " << obj.morph_count << endl;
	return out;
}

//Transform
std::ostream& operator<<(std::ostream& out, const TransformData& obj)
{
	out << "Transform name length: " << obj.name_Length << endl
		<< "ParentID: " << obj.parentID << endl
		<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
		<< "Rotation: " << obj.rotation[0] << ' ' << obj.rotation[1] << ' ' << obj.rotation[2] << ' ' << obj.rotation[3] << endl
		<< "Scale: " << obj.scale[0] << ' ' << obj.scale[1] << ' ' << obj.scale[2] << endl;
	return out;
}

//Camera
std::ostream& operator<<(std::ostream& out, const cameraData& obj)
{
	out << "Camera name length: " << obj.name_length << endl
		<< "Number of parents: " << obj.nrOfParents << endl;
	out << "CAMERA" << endl;
	out << "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
		<< "Up Vector: " << obj.up_vector[0] << ' ' << obj.up_vector[1] << ' ' << obj.up_vector[2] << endl
		<< "Intrest position: " << obj.interest_position[0] << ' ' << obj.interest_position[1] << ' ' << obj.interest_position[2] << endl
		<< "FOV X: " << obj.field_of_view_x << endl
		<< "FOV Y: " << obj.field_of_view_y << endl
		<< "Near Plane: " << obj.near_plane << endl
		<< "Far Plane: " << obj.far_plane << endl
		<< "Projection type: " << obj.projection << endl;
	return out;
}

//Mateial
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
		;
	//	<< "Name: " << obj.node_name << endl
	//	<< "Diffuse map: ";
	//if (obj.diffuse_map != nullptr)
	//	out << obj.diffuse_map;
	//out << endl
	//	<< "Normal map: ";
	//if (obj.normal_map != nullptr)
	//	out << obj.normal_map;
	//out << endl
	//	<< "Specular map: ";
	//if (obj.specular_map != nullptr)
	//	out << obj.specular_map;
	//out << endl;
	return out;
}

// Light 
std::ostream& operator<<(std::ostream& out, const LightData& obj)
{
	out << "LIGHT" << endl
		<< "Light name length: " << obj.name_Length << endl
		<< "Light type: " << obj.type << endl
		<< "Decay type: " << obj.dType << endl
		<< "Color: " << obj.color[0] << " " << obj.color[1] << " " << obj.color[2] << endl
		<< "Intensity: " << obj.intensity << endl
		<< "Cast shadow: " << obj.cast_shadows << endl
		<< "Shadow color: " << obj.shadow_color[0] << " " << obj.shadow_color[1] << " " << obj.shadow_color[2] << endl;
	return out;
}

// Nurb
std::ostream& operator << (std::ostream& out, const NurbData& obj)
{
	out << "NURB" << endl
		<< " Nurb name lengtht: " << obj.name_Length << endl
		<< " Number of nurbs: " << obj.numberOfParent << endl
		<< "Radius: " << obj.radius << endl;

	return out;

}

//struct MorphAnimation
std::ostream& operator<<(std::ostream& out, const MorphAnimationData& obj)
{
	out << "Blend Shape name length: " << obj.blendShape_name_length << endl;

	out << "nrOfWeights: " << obj.nrOfWeights << endl
		<< "nrOfTargets: " << obj.nrOfTargets << endl
		<< "nrOfVertsPerWeight: " << obj.nrOfVertsPerMesh << endl
		<< "nrOfPositions: " << obj.nrOfPositions << endl
		<< "Mesh ID: " << obj.meshID << endl;

	return out;
}

//meshStruct
std::ostream& operator<<(std::ostream& out, const meshStructData& obj)
{
	out //<< "Mesh Name: " << obj.name << endl
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

	/*for (unsigned int i = 0; i < obj.transform_count; i++)
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
	}*/

	/*int tmp = 0;
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
	}*/

	return out;
}

std::ostream& operator<<(std::ostream& out, const SkinAnimation& obj)
{
	out << "Number of influences: " << obj.numberOfInfluences << endl
		<< "Skin mesh index: " << obj.skinMeshIndex << endl
		<< "Skin vertex count: " << obj.skinVertexCount << endl;

	//for (unsigned int i = 0; i < obj.numberOfInfluences; i++)
	//{
	//	out << "Influence object " << i + 1 << " index: " << obj.influenceIndices[i] << endl;
	//}

	//for (unsigned int i = 0; i < obj.skinVertexCount; i++)
	//{
	//	out << "Vertex " << i + 1 << ": " << endl
	//		<< obj.influenceWeights[i];
	//}

	return out;
}

std::ostream& operator<<(std::ostream& out, const KeyframesData& obj)
{
	out /*<< "Curve name:" << obj.curveName << endl
		<< "Attribute name: " << obj.attachToName << endl*/
		<< "Curve name length: " << obj.curveNameLength << endl
		<< "Attribute name length: " << obj.attachToNameLength << endl
		<< "Loop the animation: " << obj.loopAnimation << endl
		<< "Affected object type: " << obj.affectedObjectType << endl
		<< "Affected object index: " << obj.affectedObjectIndex << endl
		<< "Number of keyframes: " << obj.numberOfKeyframes << endl;

	//for (unsigned int i = 0; i < obj.numberOfKeyframes; i++)
	//{
	//	out << "Keyframe " << i + 1 << ": " << endl
	//		<< obj.points[i];
	//}

	return out;
}