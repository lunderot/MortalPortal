struct PS_IN
{
	float4 Position : SV_POSITION;
	float3 Color	: COLOR;
};

float4 main( PS_IN input ) : SV_Target
{
	return float4(input.Color, 1.0f);
}