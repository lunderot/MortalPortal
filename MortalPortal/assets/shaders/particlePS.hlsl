struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(VS_OUT input) : SV_Target
{

	return float4(input.position.x, input.position.y, 1, 1);
}