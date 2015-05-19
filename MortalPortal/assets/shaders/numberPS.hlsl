
texture2D textureB : register (t0);

SamplerState sample1;

struct PS_IN
{
	float4 Position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main(PS_IN input) : SV_Target
{
	return textureB.Sample(sample1, input.texCoord);
}