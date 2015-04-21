cbuffer ConstantBufferPerFrame : register(cb0)
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ConstantBufferPerModel : register(cb1)
{
	matrix worldMatrix;
};

float4 main(float2 input : POSITION) : SV_POSITION
{

	return float4(input, 0.0f, 1.0f);
}