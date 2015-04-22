
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
	p.Pos.x -= 0.005;
	p.Color.x = 0.4;
	if (p.Pos.x < -10)
		p.Pos.x = 40;
	srcParticleBuffer[dispatchThreadID.x] = p;
}