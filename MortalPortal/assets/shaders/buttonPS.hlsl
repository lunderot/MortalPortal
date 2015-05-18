texture2D tex;
SamplerState samplState;

cbuffer buttonScale : register(cb0)
{
	matrix scaleMatrix;
	bool button;
}

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(VS_OUT input) : SV_Target
{
	float4 texColor = tex.Sample(samplState, input.texCoord);
	if (button == true)
	{
		float transparency = scaleMatrix._m00;
		if (transparency < 0.2f)
		{
			texColor.a -= 0.5f;
		}
	}
	return float4(texColor);
}