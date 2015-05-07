texture2D tex;
SamplerState samplState;
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 particlePos : PARTICLEPOS;
	float2 texCoord : TEXCOORD;
	float lifeTime : LIFETIME;
	unsigned int type : TYPE;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 texColor;
	if (input.type == 2)
	{
		
		input.texCoord.x = 1 / 0.7f * abs(input.particlePos.x);
		input.texCoord.y = 1 / 0.7f * abs(input.particlePos.y);
		texColor = tex.Sample(samplState, input.texCoord);
	}
	else if (input.type == 1)
	{
		texColor = tex.Sample(samplState, input.texCoord);
		//texColor[3] -= input.lifeTime * 0.05f;
		//texColor[3] = saturate(texColor[3]);
	}
	else
		texColor = tex.Sample(samplState, input.texCoord);
	return float4(texColor[0], texColor[1], texColor[2], texColor[3]);
}
