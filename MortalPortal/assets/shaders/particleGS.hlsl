struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};
struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

[maxvertexcount(4)]
void main(point GS_IN input[1], inout TriangleStream<GS_OUT> triStream)
{
	GS_OUT output;
	float3 camPos = float3(0, 0, -2);
	float3 look = input[0].Pos - camPos;

		look = normalize(look);

	float3 upVec = float3(0, 0.5, 0);
	float3 rightVec = normalize(cross(look, upVec));
	float3 vert[4];
	float2 texCoord[4];
	rightVec = rightVec * 0.25;

	vert[0] = (input[0].Pos - rightVec);
	vert[1] = (input[0].Pos + rightVec);
	vert[2] = (input[0].Pos - rightVec + upVec);
	vert[3] = (input[0].Pos + rightVec + upVec);

	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	for (int i = 0; i < 4; i++)
	{
		output.Pos = float4(vert[i], 1.0f);
		output.Tex = texCoord[i];
		triStream.Append(output);
	}

}