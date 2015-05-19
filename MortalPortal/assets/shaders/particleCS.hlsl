

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

	velocity.x += acceleration.x * deltaTime;
	velocity.y += acceleration.y * deltaTime;
	if (particleType == 2)
	{
		pos.x += velocity.x * deltaTime / 100;
		pos.y += velocity.y * deltaTime / 100;
	}

	else if (particleType == 3)
	{
		pos.x += velocity.x * deltaTime;
		pos.y += velocity.y * deltaTime;
	}

	else if (particleType == 4)
	{	
		float3 tmpPosition = position;
		tmpPosition.x -= 0.8f;

		if (life > lifeTime)
		{
			pos.x = tmpPosition.x;
			pos.y = tmpPosition.y;
			life -= lifeTime;
		}

		float3 vec = cross(float3(0, 0, 1), float3(velocity.x, velocity.y, 0));
		float test = lerp(1, 0.001, saturate(length(pos - tmpPosition)));
		pos.x += (test) * (velocity.x + vec.x)/2 * deltaTime;

		pos.y += (test)* (velocity.y + vec.y)/2 * deltaTime;
	}

	else if (particleType == 5)
	{
		pos.x += velocity.x * deltaTime / 20;
		pos.y += velocity.y * deltaTime / 20;

		if (life > lifeTime && pos.x < position.x)
		{
			int whatEngine = dispatchThreadID.x % 2;
			if (whatEngine == 0)
				pos.y = position.y + 2;
			else
				pos.y = position.y - 2;

			pos.x = position.x - 2;
			life -= lifeTime;
		}

		else if (pos.x > position.x - 1.0f)
		{
			int whatEngine = dispatchThreadID.x % 2;
			if (whatEngine == 0)
				pos.y = position.y + 200;
			else
				pos.y = position.y - 200;

			pos.x = position.x - 2;
			life -= lifeTime * 0.30f;
		}


	}
	else
	{
		pos.x += velocity.x * deltaTime / 5;
		pos.y += velocity.y * deltaTime / 5;
	}
	
	if (life > lifeTime && particleType == 2)
	{
		pos.x = position.x - 0.01f;
		pos.y = position.y - 0.05f;
		life -= lifeTime;
	}

	else if (pos.x < -32.0f && particleType == 3)
	{
		pos.x = position.x;
		pos.y = velocity.y * sin(life);
	}

	else if (reset == true && particleType != 5 && particleType != 4)
	{
		pos.x = position.x;
		pos.y = position.y;
		life = 0.0f;
	}
	if (particleType == 2)
		life += deltaTime * 20;
	else
		life += deltaTime;

	buffer.Store3(dispatchThreadID.x * 36, asuint(pos));
	buffer.Store(dispatchThreadID.x * 36 + 12, asuint(particleType));
	buffer.Store2(dispatchThreadID.x * 36 + 16, asuint(acceleration));
	buffer.Store2(dispatchThreadID.x * 36 + 24, asuint(velocity));
	buffer.Store(dispatchThreadID.x * 36 + 32, asuint(life));
}