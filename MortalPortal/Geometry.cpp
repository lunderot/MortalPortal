#include "Geometry.h"


Geometry::Geometry(ID3D11Buffer* vertexBuffer, unsigned int vertexCount)
{
	this->vertexBuffer = vertexBuffer;
	this->vertexCount = vertexCount;
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
