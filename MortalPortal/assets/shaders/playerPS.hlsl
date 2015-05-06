texture2D test : register (t0);

//normalmap texture
texture2D normalm : register (t1);

SamplerState sample1;

//lightbuffer
cbuffer Lightbuffer : register (b0)
{
	float3 positionC;
	float3 ambientC;
	float3 diffuseC;
	float3 specularC;
}

cbuffer ConstantBufferPerState
{
	unsigned int colorState;
	unsigned int colorState2;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float4 worldPos : WORLDPOS;
	float3 normal : NORMAL;
	float3 biNormal : BINORMAL;
	float3 tangent : TANGENT;
};

float4 main(VS_OUT input) : SV_Target
{

	//normal map

	float3 normalizedNormal = normalize(input.normal);
	float3 normalizedBiNormal = normalize(-input.biNormal);
	float3 normalizedTangent = normalize(input.tangent);

	matrix <float, 3, 3> TBNmatrix = { normalizedTangent.x, normalizedTangent.y, normalizedTangent.z,
						 normalizedBiNormal.x, normalizedBiNormal.y, normalizedBiNormal.z,
						 normalizedNormal.x, normalizedNormal.y, normalizedNormal.z};

	float4 withNormalMap = mul(normalm.Sample(sample1, input.texCoord), 2) - float4(1, 1, 1, 1);
	float3 withNormalMap2 = normalize(withNormalMap.xyz);
	float3 finalNormalM = mul(withNormalMap2, TBNmatrix);

	// ------------------ end


	//ljus
	float3 lightDirection = normalize(positionC);
	float3 cameraPosition = (0, 0, -20);
	float3 vecCamToFace = normalize(cameraPosition - input.worldPos.xyz);
	float3 reflection = reflect(lightDirection, finalNormalM);

	//diffuse
	float3 diffuseLight = mul(max(dot(-lightDirection, finalNormalM), 0.0f), diffuseC);
	float specPower = 1000.0f;
	float3 specularLight = mul(pow(max(dot(vecCamToFace, reflection), 0.0f), specPower), specularC);

	float3 addAmbDiffSpec = saturate(diffuseLight + specularLight + ambientC);


	// ----------- end



	float4 test3 = test.Sample(sample1, input.texCoord);

	if (colorState == 1)
	{
		return float4(1, 0, 0, 1);
	}
	else
	{
		return test3 * float4(addAmbDiffSpec, 1.0f)/* + float4(ambientC, 1.0f)*/;
		//return float4(input.texCoord, 1, 1);
	}

}