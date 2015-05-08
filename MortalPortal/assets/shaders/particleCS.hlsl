

uint rand(uint rng_state)
{
	rng_state ^= (rng_state << 13);
	rng_state ^= (rng_state >> 17);
	rng_state ^= (rng_state << 5);
	return rng_state;
}

cbuffer particleBuffer : register (cb0)
{
	float lifeTime;
	float3 position;
	float deltaTime;
	bool reset;
};

RWByteAddressBuffer buffer : register (t0);
[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint rng_state = dispatchThreadID.x;
	float3 pos = asfloat(buffer.Load3(dispatchThreadID.x * 36));
	uint particleType = asuint(buffer.Load(dispatchThreadID.x * 36 + 12));
	float2 acceleration = asfloat(buffer.Load2(dispatchThreadID.x * 36 + 16));
	float2 velocity = asfloat(buffer.Load2(dispatchThreadID.x * 36 + 24));
	float life = asfloat(buffer.Load(dispatchThreadID.x * 36 + 32));
	//velocity = normalize(velocity);

	velocity.x += acceleration.x * deltaTime;
	velocity.y += acceleration.y * deltaTime;
	if (particleType == 2)
	{
		pos.x += velocity.x * deltaTime / 100;
		pos.y += velocity.y * deltaTime / 100;
	}

	else if (particleType == 3)
	{
		float f0 = float(rand(rng_state) * 200 / 4294967296.0) + 10;
		pos.x += velocity.x * deltaTime / f0;
		pos.y += velocity.y * deltaTime / f0;
	}
	else
	{
		pos.x += velocity.x * deltaTime / 5;
		pos.y += velocity.y * deltaTime / 5;
	}
	//pos.z = 0;
	life += deltaTime * 20;

	//pos.y -= 0.3f * deltaTime;
	if (life > lifeTime && particleType == 2)
	{
		pos.x = position.x - 0.02f;
		pos.y = position.y - 0.05f;
		life -= lifeTime;
	}

	else if (pos.x < -32.0f && particleType == 3)
	{
		pos.x = position.x;
		pos.y = 0;
		life -= lifeTime;
	}

	else if (reset == true)
	{
		pos.x = position.x;
		pos.y = position.y;
		life -= lifeTime;
	}

	buffer.Store3(dispatchThreadID.x * 36, asuint(pos));
	buffer.Store(dispatchThreadID.x * 36 + 12, asuint(particleType));
	buffer.Store2(dispatchThreadID.x * 36 + 16, asuint(acceleration));
	buffer.Store2(dispatchThreadID.x * 36 + 24, asuint(velocity));
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