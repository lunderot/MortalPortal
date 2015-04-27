cbuffer particleBuffer : register (cb0)
{
	float maxRange;
	float3 position;
	float2 velocity;
	float2 accelaration;
	float deltaTime;
};
struct Particle
{
	float3 Pos;
	float2 Color;
};

RWByteAddressBuffer buffer : register (t0);
[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	float3 pos = asfloat(buffer.Load3(dispatchThreadID.x * 20));
	pos.x -= 0.1f;

	if (abs(position.x - pos.x) > maxRange)
	{
		pos.x = position.x;
		pos.y = position.y;
	}
	buffer.Store3(dispatchThreadID.x * 20, asuint(pos));
}


//RWStructuredBuffer<Particle> srcParticleBuffer : register(u0);
//[numthreads(10, 2, 2)]
//void main(uint3 dispatchThreadID : SV_DispatchThreadID)
//{
//	Particle p = srcParticleBuffer[dispatchThreadID.x];
//	p.Pos.x -= 0.02f;
//	p.Pos.y = position.y;
//
//	if (abs(position.x - p.Pos.x) > maxRange)
//	{
//		p.Pos.x = position.x;
//		p.Pos.y = position.y;
//	}
//
//	srcParticleBuffer[dispatchThreadID.x] = p;
//}