
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
	p.Pos.x -= 0.1f;
	p.Color.x = 0.4;

	srcParticleBuffer[dispatchThreadID.x] = p;
}