texture2D tex;
SamplerState samplState;

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 texColor = tex.Sample(samplState, input.texCoord);
	return float4(texColor);
	//return float4(input.texCoord.x, input.texCoord.y, 1, 1);
}