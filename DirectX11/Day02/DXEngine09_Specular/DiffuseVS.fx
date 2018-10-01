// �������.
cbuffer cbPerObject
{
	matrix world;
	matrix view;
	matrix projection;
};

// ����Ʈ
cbuffer cbLight
{
	float4 worldLightPosition;
};

// ���� ���̴� �Է¿� ����ü
struct vs_input
{
	float4 pos:POSITION;
	float2 texCoord:TEXCOORD0;
	float3 normal:NORMAL;
};

// ���� ���̴� ��¿� ����ü.
struct vs_output
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal:NORMAL;
	float3 diffuse: TEXCOORD1;
};

// ���� ���̴�.
vs_output main(vs_input input)
{
	vs_output output;
	// ���� ��ȯ.
	output.pos = mul(input.pos, world);		// ���� ��ȯ.
	output.pos = mul(output.pos, view);		// �� ��ȯ.
	output.pos = mul(output.pos, projection);		// ���� ��ȯ.


	// ����Ʈ����
	float3 lightDir = output.pos - worldLightPosition;
	lightDir = normalize(lightDir);

	float3 worldNormal = mul(input.normal, (float3x3)world);
	float3 diffuse = dot(-lightDir.xyz, worldNormal);


	// �������� �Է¹��� �ؽ�ó ��ǥ ��ȯ.
	output.texCoord = input.texCoord;
	output.normal = worldNormal;
	output.diffuse = diffuse * 0.5f + 0.5f;
	return output;
}