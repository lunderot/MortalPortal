texture2D tex;
SamplerState samplState;
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 particlePos : PARTICLEPOS;
	float2 texCoord : TEXCOORD;
	float lifeTime : LIFETIME;
};

float4 main(VS_OUT input) : SV_Target
{
	input.texCoord.x = 1 / 0.6f * abs(input.particlePos.x);
	input.texCoord.y = 1 / 0.6f * abs(input.particlePos.y);
	float4 texColor = tex.Sample(samplState, input.texCoord);
	return float4(texColor[0], texColor[1], texColor[2], texColor[3] - 1 / input.lifeTime);
}