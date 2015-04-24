//mange
cbuffer ConstantBufferPerState
{
	unsigned int colorState;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_Target
{
	return float4(1, 0, 0, 1);
}