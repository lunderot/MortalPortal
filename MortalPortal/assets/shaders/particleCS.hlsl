cbuffer particleBuffer : register (cb0)
{
	float lifeTime;
	float3 position;
	float2 velocity;
	float2 accelaration;
	float deltaTime;
};
struct Particle
{
	float3 Pos;
	float type;
	float lifeTime;
	float speed;
	float lifeTimeCount;
};

RWByteAddressBuffer buffer : register (t0);
[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	float3 pos = asfloat(buffer.Load3(dispatchThreadID.x * 12));
	float particleType = asfloat(buffer.Load(dispatchThreadID.x * 4));
	float life = asfloat(buffer.Load(dispatchThreadID.x * 4));
	float speed = asfloat(buffer.Load(dispatchThreadID.x * 4));

	//velocity.x += 0.3f * deltaTime;
	pos.x -= 0.3f * deltaTime;

	if (velocity.x > lifeTime)
	{
		pos.x = position.x;
		pos.y = position.y;
	}

	buffer.Store3(dispatchThreadID.x * 12, asuint(pos));
	buffer.Store(dispatchThreadID.x * 4, asuint(particleType));
	buffer.Store(dispatchThreadID.x * 4, asuint(life));
	buffer.Store(dispatchThreadID.x * 4, asuint(speed));
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