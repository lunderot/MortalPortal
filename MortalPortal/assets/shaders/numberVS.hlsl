

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
	float4 Position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Position = float4(input.position, 1.0f);
	output.Position = mul(output.Position, worldMatrix);
	output.Position = mul(output.Position, viewMatrix);
	output.Position = mul(output.Position, projectionMatrix);

	output.texCoord = input.texCoord;

	return output;
}