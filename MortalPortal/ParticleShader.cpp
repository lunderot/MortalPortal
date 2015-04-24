#include "ParticleShader.h"

ParticleShader::ParticleShader(LPCWSTR computeShaderPath, LPCWSTR geometryShaderPath, ID3D11Device* device, LPWSTR vertexShaderPath, LPWSTR pixelShaderPath, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar)
	: Shader(device, screenWidth, screenHeight, screenNear, screenFar)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD0", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateMandatoryShaders(device, vertexShaderPath, pixelShaderPath, inputDesc, ARRAYSIZE(inputDesc));
	CreateShaders(device, computeShaderPath, geometryShaderPath);
}    

void ParticleShader::CreateShaders(ID3D11Device* device, LPCWSTR computeShaderFilename, LPCWSTR geometryShaderFilename)
{
	HRESULT hr;
	ID3DBlob* errorMessage = nullptr;

	// Create Compute Shader
	ID3DBlob* pCS = nullptr;
	hr = D3DCompileFromFile(computeShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", NULL, NULL, &pCS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw std::runtime_error(std::string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			throw std::runtime_error("No such file");
		}
	}

	device->CreateComputeShader(pCS->GetBufferPointer(), pCS->GetBufferSize(), nullptr, &computeShader);
	pCS->Release();

	// Create Geometry Shader
	ID3DBlob* pGS = nullptr;
	hr = D3DCompileFromFile(geometryShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "gs_4_0", NULL, NULL, &pGS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw std::runtime_error(std::string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			throw std::runtime_error("No such file");
		}
	}

	device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &geometryShader);
	pGS->Release();
}

void ParticleShader::Use(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(hullShader, nullptr, 0);
	deviceContext->DSSetShader(domainShader, nullptr, 0);
	deviceContext->GSSetShader(geometryShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->GSSetConstantBuffers(0, 1, &constantBufferPerFrame);
	deviceContext->GSSetConstantBuffers(1, 1, &constantBufferPerModel);
}

ID3D11ComputeShader* ParticleShader::GetComputeShader()
{
	return computeShader;
}

ParticleShader::~ParticleShader()
{
	if (computeShader)
		computeShader->Release();
}