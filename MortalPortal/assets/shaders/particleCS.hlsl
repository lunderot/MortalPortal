cbuffer particleBuffer : register (cb0)
{
	float lifeTime;
	float3 position;
	float2 velocity;
	float2 accelaration;
	float deltaTime;
};

RWByteAddressBuffer buffer : register (t0);
[numthreads(10, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	float3 pos = asfloat(buffer.Load3(dispatchThreadID.x * 32));
	float particleType = asfloat(buffer.Load(dispatchThreadID.x * 32 + 12));
	float2 direction = asfloat(buffer.Load2(dispatchThreadID.x * 32 + 16));
	float life = asfloat(buffer.Load(dispatchThreadID.x * 32 + 24));
	float speed = asfloat(buffer.Load(dispatchThreadID.x * 32 + 28));

	//velocity.x += 0.3f * deltaTime;
	pos.x -= 8.0f * deltaTime;
	life += 8.0f * deltaTime;
	//pos.y -= 0.3f * deltaTime;

	if (life > lifeTime)
	{
		pos.x = position.x;
		pos.y = position.y;
		life -= lifeTime;
	}

	buffer.Store3(dispatchThreadID.x * 32, asuint(pos));
	buffer.Store(dispatchThreadID.x * 32 + 12, asuint(particleType));
	buffer.Store2(dispatchThreadID.x * 32 + 16, asuint(direction));
	buffer.Store(dispatchThreadID.x * 32 + 24, asuint(life));
	buffer.Store(dispatchThreadID.x * 32 + 28, asuint(speed));
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