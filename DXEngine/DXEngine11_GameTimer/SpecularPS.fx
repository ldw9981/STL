// �ȼ� ���̴� �Է¿� ����ü.
struct ps_input
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 diffuse : TEXCOORD1;

	float4 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

// �ؽ�ó ����.
Texture2D objTexture;
SamplerState objSampler;

// �ȼ� ���̴�(���̴�/���̴�).
float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó ���ø� (���� ���� ����).
	float4 texColor = objTexture.Sample(
		objSampler, input.texCoord);

	// Diffuse ���� ����.
	float3 diffuse = saturate(input.diffuse);

	// ����ŧ�� ���.
	float3 reflection = normalize(input.reflection);
	float3 viewDir = normalize(input.viewDir.xyz);
	float3 specular = 0;

	if (diffuse.x > 0)
	{
		// ���� (�� ����, �ݻ纤��).
		specular = dot(reflection, -viewDir);
		// 0-1 ����.
		specular = saturate(specular);
		// ���ݻ� ���� ���̰� �� ������Ű��.
		specular = pow(specular, 20.0f);
	}

	// ���� ����.
	float3 finalColor = texColor.rgb * diffuse + specular;

	//return float4(finalColor, 1);
	return float4(specular, 1);
}