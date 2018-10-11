// �ȼ� ���̴� �Է� ����ü.
struct ps_input
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;

	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

// �ؽ�ó ���� ���ҽ�.
Texture2D diffuseMap : register (t0);
Texture2D normalMap : register (t1);
SamplerState texSampler : register(s0);

float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó���� �о�� ���.
	float3 tangentNormal
		= normalMap.Sample(texSampler, input.texCoord);
	tangentNormal = normalize(tangentNormal * 2 - 1);
	// Test.
	//tangentNormal = float3(0.0f, 0.0f, 1.0f);

	// TBN ��� �����.
	float3x3 TBN = float3x3(
		normalize(input.T),
		normalize(input.B),
		normalize(input.N)
	);

	// ���� ��� �� ���ϱ�.
	float3 worldNormal
		= normalize(mul(tangentNormal, transpose(TBN)));

	// ����Ʈ ����.
	float3 lightDir = normalize(input.lightDir);

	// ���ݻ籤 ���ϱ�.
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	float toonStep = 3.0f;
	diffuse = ceil(diffuse * toonStep) / toonStep;
	// ���� ����Ʈ(Half Lambert).
	//diffuse = pow(diffuse * 0.5f + 0.5f, 2.0f);

	// ��ǻ��� �б�.
	float4 albedo = diffuseMap.Sample(texSampler, input.texCoord);

	// ���� ��ǻ�� ����.
	diffuse = diffuse * albedo;

	// ���ݻ籤(specular).
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		// �ݻ纤��/�� ���� ���ϱ�.
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(input.viewDir);

		specular = dot(reflection, -viewDir);
		specular = saturate(specular);
		specular = pow(specular, 20.0f);
	}

	// �ں��Ʈ ����Ʈ.
	float3 ambient = float3(0.1f, 0.1f, 0.1f);

	// ���� ���� ��ȯ.
	return float4(diffuse + specular, 1);
}