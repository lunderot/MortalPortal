#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <stdexcept>
#include <DirectXMath.h>

struct ConstantBufferPerFrame
{
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

struct ConstantBufferPerModel
{
	DirectX::XMFLOAT4X4 worldMatrix;
};

//mange
struct ConstantBufferPerState
{
	UINT colorState;
	float pad1;
	float pad2;
	float pad3;
};

class Shader
{
protected:
	ID3D11VertexShader*		vertexShader;
	ID3D11PixelShader*		pixelShader;
	ID3D11HullShader*		hullShader;
	ID3D11GeometryShader*	geometryShader;
	ID3D11DomainShader*		domainShader;
	ID3D11ComputeShader*	computeShader;

	ID3D11InputLayout*		inputLayout;

	ConstantBufferPerFrame	constantBufferPerFrameData;
	ConstantBufferPerModel	constantBufferPerModelData;
	
	ID3D11Buffer*			constantBufferPerFrame;
	ID3D11Buffer*			constantBufferPerModel;
	
	//mange
	ID3D11Buffer*			constantBufferPerState;

public:

	Shader(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar);
	virtual ~Shader();

	void CreateMandatoryShaders(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename, D3D11_INPUT_ELEMENT_DESC* inputDesc, unsigned int inputDescSize);
	void CreateComputeShader(ID3D11Device* device, LPCWSTR computeShaderFilename);
	void Use(ID3D11DeviceContext* deviceContext);
	void UpdateConstantBufferPerFrame(ID3D11DeviceContext* deviceContext, ConstantBufferPerFrame* buffer);
	void UpdateConstantBufferPerModel(ID3D11DeviceContext* deviceContext, ConstantBufferPerModel* buffer);

	//mange
	ConstantBufferPerState	constantBufferPerStateData;
	void UpdateConstantBufferPerState(ID3D11DeviceContext* deviceContext, ConstantBufferPerState* buffer);
};

