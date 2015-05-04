cbuffer particleBuffer : register (cb0)
{
	float lifeTime;
	float3 position;
	float deltaTime;
};

RWByteAddressBuffer buffer : register (t0);
[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	float3 pos = asfloat(buffer.Load3(dispatchThreadID.x * 36));
	float particleType = asfloat(buffer.Load(dispatchThreadID.x * 36 + 12));
	float2 acceleration = asfloat(buffer.Load2(dispatchThreadID.x * 36 + 16));
	float2 velocity = asfloat(buffer.Load2(dispatchThreadID.x * 36 + 24));
	float life = asfloat(buffer.Load(dispatchThreadID.x * 36 + 32));


	float2 vel = velocity;
	vel.x += acceleration.x * deltaTime;
	vel.y += acceleration.y * deltaTime;
	pos.x += vel.x * deltaTime;
	pos.y += vel.y * deltaTime;
	pos.z = 0;

	life += 8.0f * deltaTime;
	//pos.y -= 0.3f * deltaTime;

	if (life > lifeTime)
	{
		pos.x = position.x;
		pos.y = position.y;
		life -= lifeTime;
	}

	buffer.Store3(dispatchThreadID.x * 36, asuint(pos));
	buffer.Store(dispatchThreadID.x * 36 + 12, asuint(particleType));
	buffer.Store2(dispatchThreadID.x * 36 + 16, asuint(acceleration));
	buffer.Store2(dispatchThreadID.x * 36 + 24, asuint(vel));
	buffer.Store(dispatchThreadID.x * 36 + 32, asuint(life));
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