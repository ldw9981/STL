// �ȼ� ���̴� �Է¿� ����ü.
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

// �ؽ�ó ����.
Texture2D diffuseMap: register(t0);
Texture2D normalMap : register(t1);
SamplerState texSampler;

// �ȼ� ���̴�(���̴�/���̴�).
float4 main(ps_input input) : SV_TARGET
{
	float3 tangentNormal = normalMap.Sample(texSampler,input.texCoord);
	tangentNormal = normalize(tangentNormal * 2 - 1); // 0~1 -> -1 ~ 1�� ����

	// TBN ��� �����
	float3x3 TBN = float3x3(
		normalize(input.T),
		normalize(input.B),
		normalize(input.N)
		);

	//���� �����̽� ���
	float3 worldNormal = normalize(mul(tangentNormal,transpose(TBN)));
	//�����Ǿ� �ٽ� 
	float3 lightDir = normalize(input.lightDir);
	//���ݻ�
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	float4 albedo = diffuseMap.Sample(texSampler, input.texCoord);
	diffuse = diffuse * albedo;

	//���ݻ�
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, WorldNormal);
		float3 viewDir = normalize(intput.viewDir);

		specular = dot(reflection, -viewDir);
		specular = saturate(specular);
		specular = pow(specular, 20.0f);
	}

	return float4(diffuse+ specular, 1.0f);
}