struct VS_IN
{
	float3 position : POSITION;
	unsigned int type : TYPE;
	float2 acceleration : ACCELERATION;
	float2 velocity : VELOCITY;
	float lifeTime : LIFETIME;
};

struct VS_OUT
{
	float4 position : POSITION;
	unsigned int type : TYPE;
	float2 velocity : VELOCITY;
	float lifeTime : LIFETIME;
	float speed : SPEED;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.position = float4(input.position, 1.0f);

	output.type = input.type;
	output.lifeTime = input.lifeTime;
	output.velocity = input.velocity;

	return output;
}