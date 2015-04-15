cbuffer ConstantBufferPerFrame : register(cb0)
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ConstantBufferPerModel : register(cb1)
{
	matrix worldMatrix;
};

struct VS_IN
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.position = float4(input.position, 1.0f);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.texCoord = input.texCoord;

	return output;
}