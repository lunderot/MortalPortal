#include "Geometry.h"


Geometry::Geometry(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, Collision* collision)
{
	this->vertexBuffer = vertexBuffer;
	this->vertexCount = vertexCount;
	this->collision = collision;
}


Geometry::~Geometry()
{
}


ID3D11Buffer* Geometry::GetVertexBuffer() const
{
	return vertexBuffer;
}
unsigned int Geometry::GetVertexCount() const
{
	return vertexCount;
}

Collision* Geometry::GetCollision() const
{
	return collision;
}
