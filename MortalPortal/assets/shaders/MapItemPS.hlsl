texture2D tex : register(t0);
SamplerState samplState;

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(PS_IN input) : SV_Target
{
	float3 texColor = tex.Sample(samplState, input.texCoord);
	return float4(texColor, 1);
}