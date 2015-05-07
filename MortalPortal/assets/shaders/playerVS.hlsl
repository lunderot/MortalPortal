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
	float3 normal : NORMAL;
	float3 biNormal : BINORMAL;
	float3 tangent : TANGENT;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float4 worldPos : WORLDPOS;
	float3 normal : NORMAL;
	float3 biNormal : BINORMAL;
	float3 tangent : TANGENT;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.position = float4(input.position, 1.0f);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	//calculate worldpos
	output.worldPos = mul(float4(input.position, 1.0f), worldMatrix);
	output.normal = input.normal;
	output.biNormal = input.biNormal;
	output.tangent = input.tangent;

	output.texCoord = input.texCoord;


	return output;
}