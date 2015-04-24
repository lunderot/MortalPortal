texture2D test;
SamplerState test1;

cbuffer ConstantBufferPerState
{
	unsigned int colorState;
	unsigned int colorState2;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_Target
{
	float3 test3 = test.Sample(test1, input.texCoord);

	if (colorState == 1)
	{
		return float4(1, 0, 0, 1);
	}
	else
	{
		return float4(test3, 1.0f);
		//return float4(input.texCoord, 1, 1);
	}

}