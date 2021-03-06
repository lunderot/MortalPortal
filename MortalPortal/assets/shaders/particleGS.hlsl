cbuffer ConstantBufferPerFrame : register(cb0)
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ConstantBufferPerModel : register(cb1)
{
	matrix worldMatrix;
};

struct GS_IN
{
	float4 Pos : POSITION;
	unsigned int type : TYPE;
	float2 direction : DIRECTION;
	float lifeTime : LIFETIME;
	float speed : SPEED;
};
struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float4 particlePos : PARTICLEPOS;
	float2 Tex : TEXCOORD0;
	float lifeTime : LIFETIME;
	unsigned int type : TYPE;
};

[maxvertexcount(4)]
void main(point GS_IN input[1], inout TriangleStream<GS_OUT> triStream)
{
	GS_OUT output;
	float3 camPos = float3(0, 0, -20);
	float3 look = float3(0, 0, 20);//input[0].Pos - camPos;
	output.particlePos = input[0].Pos;
	output.type = input[0].type;
	output.lifeTime = input[0].lifeTime;

	look = normalize(look);

	float3 upVec = float3(0, 1, 0);
	float3 rightVec = normalize(cross(look, upVec));
	upVec = normalize(cross(rightVec, look));
	float3 vert[4];
	float2 texCoord[4];
	if (input[0].type == 1)
	{
		float size = lerp(0.4, 0, input[0].lifeTime / 0.5f);
		rightVec = rightVec * size;
		upVec = upVec * size;

	}
	else if (input[0].type == 2)
	{	
		float size = lerp(0.005, 0, input[0].lifeTime / 20.0f);
		rightVec = rightVec * size;
		upVec = upVec * size;
	}
	else if (input[0].type == 4)
	{
		float size = lerp(0.5, 0, input[0].lifeTime / 0.3f);
		rightVec = rightVec * size;
		upVec = upVec * size;
		
	}

	else if (input[0].type == 5)
	{
		float size = lerp(0.3, 0.0, input[0].lifeTime / 0.3f);
		rightVec = rightVec * size;
		upVec = upVec * size;

	}
	else
	{
		rightVec = rightVec * 0.05f;
		upVec = upVec * 0.05f;
	}


	vert[0] = (input[0].Pos - rightVec - upVec);
	vert[1] = (input[0].Pos + rightVec - upVec);
	vert[2] = (input[0].Pos - rightVec + upVec);
	vert[3] = (input[0].Pos + rightVec + upVec);

	texCoord[0] = float2(0, 1);
	texCoord[1] = float2(1, 1);
	texCoord[2] = float2(0, 0);
	texCoord[3] = float2(1, 0);

	if (input[0].type == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			output.Pos = float4(vert[i].x, vert[i].y, 0.5, 1.0f);
			output.Tex = texCoord[i];
			triStream.Append(output);
		}
	}

	else
	{
		for (int i = 0; i < 4; i++)
		{
			output.Pos = mul(float4(vert[i], 1.0f), viewMatrix);
			output.Pos = mul(output.Pos, projectionMatrix);
			output.Tex = texCoord[i];
			triStream.Append(output);
		}
	}

}