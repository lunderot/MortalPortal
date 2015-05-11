
texture2D textureB : register (t0);

SamplerState sample1;

struct PS_IN
{
	float4 Position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

float4 main( PS_IN input ) : SV_Target
{
	//input.Position.xy /= input.Position.w;

	//float test1 = 0.0f;
	//if ((-0.5f * input.Position.y + 0.5f) < 0.5f)
	//{
	//	test1 = 1.0f;
	//}
	//else
	//{
	//	test1 = -1.0f;
	//}

	//float2 test2 = float2(input.texCoord.x, test3 * test);// (test1 * test * 0.5));
	float4 sampleBackground = textureB.Sample(sample1, input.texCoord);
	return sampleBackground;
	//return float4(input.Color, 1.0f);
}