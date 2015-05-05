#include "PowerBar.h"

PowerBar::PowerBar(ID3D11Device* device)
{
	Points p[4] =
	{
		DirectX::XMFLOAT2(-0.1f, 1.0f),
		DirectX::XMFLOAT2(0.0f, 1.0f),

		DirectX::XMFLOAT2(-0.1f, 0.9f),
		DirectX::XMFLOAT2(0.0f, 0.0f),

		DirectX::XMFLOAT2(-0.7f, 1.0f),
		DirectX::XMFLOAT2(1.0f, 1.0f),

		DirectX::XMFLOAT2(-0.7f, 0.9f),
		DirectX::XMFLOAT2(1.0f, 0.0f)
	};

	points[0] = p[0];
	points[1] = p[1];
	points[2] = p[2];
	points[3] = p[3];

	maxMinValue.x = -0.1f;
	maxMinValue.y = -0.7f;
	powerAdd = 0.02f;
	powerRemove = 0.04f;
	dead = false;
	material = nullptr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * 4;
	bufferDesc.StructureByteStride = sizeof(DirectX::XMFLOAT4);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &points;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create vertex buffer in the PowerBar class.");
	}

	barSpeed = -0.01f;
}
void PowerBar::SetBarSpeed(float speed)
{
	this->barSpeed = speed;
}
void PowerBar::SetPosition(DirectX::XMFLOAT2 point[4])
{
	this->points[0].pos = point[0];
	this->points[1].pos = point[1];
	this->points[2].pos = point[2];
	this->points[3].pos = point[3];
}

void PowerBar::SetMaxMinValue(DirectX::XMFLOAT2 value)
{
	this->maxMinValue.x = value.x;
	this->maxMinValue.y = value.y;
}
const bool PowerBar::IsDead()
{
	return dead;
}

const float PowerBar::GetBarSpeed()
{
	return barSpeed;
}

const DirectX::XMFLOAT2 PowerBar::GetMaxMinValue()
{
	return maxMinValue;
}

void PowerBar::SetMaterial(Material* material)
{
	this->material = material;
}

ID3D11Buffer* PowerBar::GetVertexBuffer()
{
	return vertexBuffer;
}

void PowerBar::Render(ID3D11DeviceContext* deviceContext, Shader* shader)
{
	unsigned int vertexSize = sizeof(float) * 4;
	unsigned int offset = 0;
	unsigned int vertexCount = 4;
	ID3D11Buffer* vb = vertexBuffer;
	ID3D11ShaderResourceView* srv = material->GetTexture();
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT result;
	result = deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);


	memcpy(resource.pData, &points, sizeof(points));
	deviceContext->Unmap(vertexBuffer, 0);

	deviceContext->IASetVertexBuffers(0, 1, &vb, &vertexSize, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	deviceContext->PSSetShaderResources(0, 1, &srv);
	deviceContext->Draw(vertexCount, 0);

}

void PowerBar::AddPower(unsigned int bonusPower)
{
	if (bonusPower == 0)
		bonusPower = 1;
	if (dead == false)
	{
		if (points[0].pos.x + powerAdd * (bonusPower + 1) > maxMinValue.x)
		{
			points[0].pos.x = maxMinValue.x;
			points[1].pos.x = maxMinValue.x;
		}
		else
		{
			points[0].pos.x  += powerAdd * (bonusPower +1);
			points[1].pos.x  += powerAdd * (bonusPower + 1);
		}
	}
}

void PowerBar::RemovePower()
{
	if (points[0].pos.x - powerRemove < maxMinValue.y)
	{
		points[0].pos.x = maxMinValue.y;
		points[1].pos.x = maxMinValue.y;
	}
	else
	{
		points[0].pos.x -= powerRemove;
		points[1].pos.x -= powerRemove;
	}
}


void PowerBar::Update(float deltaTime, ID3D11DeviceContext* deviceContext)
{

	if (points[0].pos.x <= maxMinValue.y && dead == false)
	{
		points[0].pos.x = maxMinValue.y;
		dead = true;
	}
	else if (points[0].pos.x  > maxMinValue.x && dead == false)
	{
		points[0].pos.x = maxMinValue.x;	
		points[1].pos.x = maxMinValue.x;	
	}
	else if (dead == false)
	{
		points[0].pos.x += barSpeed * deltaTime;
		points[0].uv.x = 1 / (maxMinValue.x - maxMinValue.y) * abs(points[0].pos.x);
		points[1].pos.x += barSpeed * deltaTime;
		points[1].uv.x = 1 / (maxMinValue.x - maxMinValue.y) * abs(points[1].pos.x);
	}

}

void PowerBar::Reset()
{
	this->points[0].pos.x = maxMinValue.x;
	this->points[1].pos.x = maxMinValue.x;
	this->dead = false;
}

PowerBar::~PowerBar()
{
	if (vertexBuffer)
	{
		vertexBuffer->Release();
	}
}
