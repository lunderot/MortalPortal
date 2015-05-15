texture2D texturem : register (t0);

texture2D normalm : register (t1);

texture2D specularm : register (t2);

SamplerState sample1;

//lightbuffer
cbuffer Lightbuffer : register (b0)
{
	float3 positionC;
	int isDirectional;
	float3 colorC;
}

cbuffer Materialbuffer : register (b1)
{
	float3 specular;
	float specularFactor;
	float3 ambient;
	float3 diffuse;
	float3 transparencyColor;
	float normalDepth;
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
	float3 cameraPosition = float3(0, 0, -20);
	float3 vecCamToFace = normalize(cameraPosition - input.worldPos.xyz);
	float3 reflection = reflect(lightDirection, finalNormalM);

	//diffuse
	float3 diffuseLight = mul(max(dot(-lightDirection, finalNormalM), 0.0f), colorC);
	float3 specularLight = mul(pow(max(dot(vecCamToFace, reflection), 0.0f), (specularFactor * 10.0f)), max(specularm.Sample(sample1, input.texCoord).xyz, specular));

	float3 addAmbDiffSpec = saturate(diffuseLight + ambient);


	// ----------- end



	float4 outputTextured = texturem.Sample(sample1, input.texCoord);

	if (colorState == 1)
	{
		return float4(1, 0, 0, 1);
	}
	else
	{
		return saturate(outputTextured * float4(addAmbDiffSpec, 1.0f) + float4(specularLight, 0.0f));
	}
}