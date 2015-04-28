

//struct ConstantBufferPerFrame : register (cb0)
//{
//	DirectX::XMFLOAT4X4 viewMatrix;
//	DirectX::XMFLOAT4X4 projectionMatrix;
//};
//
//
//cbuffer ConstantBufferPerModel : register(cb1)
//{
//	matrix worldMatrix;
//};


struct VS_IN
{
	float3 Position : POSITION;
	float3 Color	: COLOR;
};

struct VS_OUT
{
	float4 Position : SV_POSITION;
	float3 Color	: COLOR;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	//float4 worldPos = mul(float4())

	output.Position = float4(input.Position, 1.0f);

	output.Color = input.Color;

	return output;
}