cbuffer buttonScale : register(cb0)
{
	matrix scaleMatrix;
	bool button;
}
struct VS_IN
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.position = float4(input.position, 0.0f, 1.0f);
	if (button == true)
		output.position = mul(output.position,scaleMatrix);

	output.texCoord = input.texCoord;

	return output;
}