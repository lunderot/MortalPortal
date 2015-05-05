texture2D tex;
SamplerState samplState;
struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float lifeTime : LIFETIME;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 texColor = tex.Sample(samplState, input.texCoord);
	return float4(texColor[0], texColor[1], texColor[2], 1.0f / input.lifeTime);
}