#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Geometry.h"
#include "Material.h"
#include "Shader.h"

class Entity
{
	friend class EntityHandler;
protected:

	Geometry* geometry;
	Material* material;
	Shader* shader;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 angleVelocity;
	DirectX::XMFLOAT3 acceleration;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	bool alive;

public:
	Entity(Geometry* geometry, Material* material, Shader* shader,
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 angleVelocity = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3( 1, 1, 1)
		);
	virtual ~Entity();

	virtual void Update(float deltaTime);

	virtual Geometry* GetGeometry() const;
	virtual Material* GetMaterial() const;
	virtual Shader* GetShader() const;

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetVelocity(DirectX::XMFLOAT3 velocity);
	void SetAngleVelocity(DirectX::XMFLOAT3 angleVelocity);
	void SetAcceleration(DirectX::XMFLOAT3 acceleration);
	void SetRotation(DirectX::XMFLOAT3 rotation);
	void SetScale(DirectX::XMFLOAT3 scale);

	void SetAlive(bool alive);

	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetVelocity() const;
	DirectX::XMFLOAT3 GetAngleVelocity() const;
	DirectX::XMFLOAT3 GetAcceleration() const;
	DirectX::XMFLOAT3 GetRotation() const;
	DirectX::XMFLOAT3 GetScale() const;
	bool GetAlive() const;

};

enum Color
{
	RED,
	GREEN,
	BLUE,
	YELLOW
};

