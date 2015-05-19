texture2D tex;
texture2D alphaTex;
SamplerState samplState;
cbuffer particleBuffer : register (cb0)
{
	float lifeTime;
	float3 position;
	float deltaTime;
	bool reset;
};

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
		float4 coolAlpha;
		coolAlpha = alphaTex.Sample(samplState, input.texCoord);

		input.texCoord.x = 1 / 0.502f * abs(input.particlePos.x + 0.184);
		input.texCoord.y = 0.5f;
		texColor = tex.Sample(samplState, input.texCoord);
		texColor.a = coolAlpha.a - lerp(0, 0.5, input.lifeTime / 20.0f);
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

	else if (input.type == 5)
	{
		texColor = tex.Sample(samplState, input.texCoord);
		//texColor.r = lerp(0.3, 1, input.lifeTime / 0.1f);
		//texColor.g = lerp(0.3, 1, input.lifeTime / 0.2f);
		//texColor.b = saturate(lerp(0.7, 0.0, input.lifeTime / 0.05f));
		if (reset == true)
		{
			texColor.a = 0;
		}
		else
		{
			texColor.x += input.lifeTime * 2.5f;
			texColor.y += input.lifeTime * 2.5f;
			texColor.z += input.lifeTime * 2.5f;
			texColor.a -= lerp(0, 0, input.lifeTime / 0.5f);
		}
	}
	else
		texColor = tex.Sample(samplState, input.texCoord);

	texColor.a = saturate(texColor.a);
	return texColor;
}
