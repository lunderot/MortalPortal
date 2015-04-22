struct VS_IN
{
	float3 Position : POSITION;
	float3 Color	: COLOR;
};

struct VS_OUT
{
	float4 Position : SV_POSITION;
	float3 Color	: COLOR;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Position = float4(input.Position, 1.0f);

	output.Color = input.Color;

	return output;
}