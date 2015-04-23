#pragma once

#include <d3d11.h>

class Geometry
{
private:
	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;
public:
	Geometry(ID3D11Buffer* vertexBuffer, unsigned int vertexCount);
	~Geometry();

	ID3D11Buffer* GetVertexBuffer() const;
	unsigned int GetVertexCount() const;
};

