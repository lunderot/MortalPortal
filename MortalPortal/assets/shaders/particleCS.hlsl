cbuffer particleBuffer : register (cb0)
{
	int maxRange;
	float3 position;
	float2 velocity;
	float2 accelaration;
};
struct Particle
{
	float3 Pos;
	float2 Color;
};

RWStructuredBuffer<Particle> srcParticleBuffer : register(u0);
[numthreads(1024, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	Particle p = srcParticleBuffer[dispatchThreadID.x];
	p.Pos.x -= 0.01f;
	p.Pos.y = position.y;

	if (abs(position.x - p.Pos.x) > maxRange)
	{
		p.Pos.x = position.x;
		p.Pos.y = position.y;
	}

	srcParticleBuffer[dispatchThreadID.x] = p;
}