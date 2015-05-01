#pragma once
#include <d3d11.h>

#include "Collision.h"

class Geometry
{
private:
	ID3D11Buffer* vertexBuffer;
	unsigned int vertexCount;
	
	Collision* collision;
public:
	Geometry(ID3D11Buffer* vertexBuffer, unsigned int vertexCount, Collision* collision);
	~Geometry();

	ID3D11Buffer* GetVertexBuffer() const;
	unsigned int GetVertexCount() const;
	Collision* GetCollision() const;
};

