struct VS_IN
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.position = float4(input.position, 1.0f);

	output.texCoord = input.texCoord;

	return output;
}