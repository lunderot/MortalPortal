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
		
		input.texCoord.x = 1 / 0.675f * abs(input.particlePos.x);
		input.texCoord.y = 0.5f;
		texColor = tex.Sample(samplState, input.texCoord);
		texColor.a -= lerp(0, 0.5, input.lifeTime / 20.0f);
	}
	else if (input.type == 4)
	{
		texColor = tex.Sample(samplState, input.texCoord);
		texColor.a -= lerp(0, 0.5, input.lifeTime / 0.2f);

	}
	else if (input.type == 1)
	{
		texColor = tex.Sample(samplState, input.texCoord);
		texColor.a -= lerp(0, 0, input.lifeTime / 0.5f);
		//texColor[3] -= input.lifeTime * 0.05f;
		//texColor[3] = saturate(texColor[3]);
	}
	else
		texColor = tex.Sample(samplState, input.texCoord);

	texColor.a = saturate(texColor.a);
	return texColor;
}
