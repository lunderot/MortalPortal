#include "Shader.h"

using namespace DirectX;


Shader::Shader(ID3D11Device* device, unsigned int screenWidth, unsigned int screenHeight, float screenNear, float screenFar, bool isOrtograhic)
{
	vertexShader = nullptr;
	hullShader = nullptr;
	pixelShader = nullptr;
	geometryShader = nullptr;
	domainShader = nullptr;

	inputLayout = nullptr;

	constantBufferPerFrame = nullptr;
	constantBufferPerModel = nullptr;

	HRESULT hr;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA data;

	//Create per frame constant buffer
	XMFLOAT3 camPos(0, 0, -20);
	XMFLOAT3 lookAt(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);

	XMMATRIX view = XMMatrixLookAtLH(XMLoadFloat3(&camPos), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));
	XMStoreFloat4x4(&constantBufferPerFrameData.viewMatrix, XMMatrixTranspose(view));

	XMMATRIX projection;
	if (isOrtograhic)
	{
		projection = XMMatrixOrthographicLH(1.0f, 1.0f, screenNear, screenFar);
	}
	else
	{
		projection = XMMatrixPerspectiveFovLH(XM_PI * 0.45f, (float)screenWidth / (float)screenHeight, screenNear, screenFar);
	}
	
	XMStoreFloat4x4(&constantBufferPerFrameData.projectionMatrix, XMMatrixTranspose(projection));

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(constantBufferPerFrameData);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	data.pSysMem = &constantBufferPerFrameData;
	hr = device->CreateBuffer(&bufferDesc, &data, &constantBufferPerFrame);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create per frame constant buffer");
	}


	//Create per model constant buffer
	XMMATRIX model = XMMatrixIdentity();
	XMStoreFloat4x4(&constantBufferPerModelData.worldMatrix, XMMatrixTranspose(model));

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(constantBufferPerModelData);
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	data.pSysMem = &constantBufferPerModelData;
	hr = device->CreateBuffer(&bufferDesc, &data, &constantBufferPerModel);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create per model constant buffer");
	}

}

Shader::~Shader()
{
	if (vertexShader)
		vertexShader->Release();

	if (pixelShader)
		pixelShader->Release();

	if (hullShader)
		hullShader->Release();

	if (geometryShader)
		geometryShader->Release();

	if (domainShader)
		domainShader->Release();

	if (inputLayout)
		inputLayout->Release();

	if (constantBufferPerFrame)
		constantBufferPerFrame->Release();

	if (constantBufferPerModel)
		constantBufferPerModel->Release();
}

void Shader::CreateMandatoryShaders(ID3D11Device* device, LPCWSTR vertexShaderFilename, LPCWSTR pixelShaderFilename, D3D11_INPUT_ELEMENT_DESC* inputDesc, unsigned int inputDescSize)
{
	HRESULT hr;
	ID3DBlob* errorMessage = nullptr;

	//Create vertex shader
	ID3DBlob* pVS = nullptr;
	hr = D3DCompileFromFile(vertexShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", NULL, NULL, &pVS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw std::runtime_error(std::string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			char tmp[256];
			size_t returnsize; //Or else VS complains...
			wcstombs_s(&returnsize, tmp, vertexShaderFilename, ARRAYSIZE(tmp));
			throw std::runtime_error("This file does not exist: " + std::string(tmp));
		}
	}
	device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &vertexShader);

	//Create vertex layout
	device->CreateInputLayout(inputDesc, inputDescSize, pVS->GetBufferPointer(), pVS->GetBufferSize(), &inputLayout);
	pVS->Release();

	//Create pixel shader.
	ID3DBlob* pPS = nullptr;
	hr = D3DCompileFromFile(pixelShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", NULL, NULL, &pPS, &errorMessage);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			throw std::runtime_error(std::string(static_cast<const char *>(errorMessage->GetBufferPointer()), errorMessage->GetBufferSize()));
		}
		else
		{
			char tmp[256];
			size_t returnsize; //Or else VS complains...
			wcstombs_s(&returnsize, tmp, vertexShaderFilename, ARRAYSIZE(tmp));
			throw std::runtime_error("This file does not exist: " + std::string(tmp));
		}
	}

	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &pixelShader);
	pPS->Release();
}

void Shader::Use(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(inputLayout);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->HSSetShader(hullShader, nullptr, 0);
	deviceContext->DSSetShader(domainShader, nullptr, 0);
	deviceContext->GSSetShader(geometryShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &constantBufferPerFrame);
	deviceContext->VSSetConstantBuffers(1, 1, &constantBufferPerModel);

}

void Shader::UpdateConstantBufferPerFrame(ID3D11DeviceContext* deviceContext, ConstantBufferPerFrame* buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(constantBufferPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, buffer, sizeof(ConstantBufferPerFrame));
	deviceContext->Unmap(constantBufferPerFrame, 0);
}

void Shader::UpdateConstantBufferPerModel(ID3D11DeviceContext* deviceContext, ConstantBufferPerModel* buffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(constantBufferPerModel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, buffer, sizeof(ConstantBufferPerFrame));
	deviceContext->Unmap(constantBufferPerModel, 0);
}
