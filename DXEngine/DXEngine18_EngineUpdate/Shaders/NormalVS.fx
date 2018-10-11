// ������� - ������ȯ���.
cbuffer cbPerObject : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

// ������� - ����Ʈ��.
cbuffer cbLight : register(b1)
{
	float4 worldLightPosition;
	float4 worldCameraPosition;
};

// ���� ���̴� �Է¿� ����ü.
struct vs_input
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// ���� ���̴� ��¿� ����ü.
struct vs_output
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;

	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

// ���� ���̴�.
vs_output main(vs_input input)
{
	vs_output output;
	// ���� ��ȯ.
	output.pos = mul(input.pos, world);		// ���� ��ȯ.

	// ����Ʈ ���� ���ϱ�.
	output.lightDir = normalize(output.pos - worldLightPosition);
	// �� ���� ���ϱ�.
	output.viewDir = normalize(output.pos - worldCameraPosition);

	// ���� ��ǥ�� ���� ��� �� ���ϱ�.
	output.N = normalize(mul(input.normal, (float3x3)world));
	// ���� ��ǥ�� ���� ź��Ʈ �� ���ϱ�.
	output.T = normalize(mul(input.tangent, (float3x3)world));
	// ���� ��ǥ�� ���� ���̳�� �� ���ϱ�.
	output.B = normalize(mul(input.binormal, (float3x3)world));

	output.pos = mul(output.pos, view);		// �� ��ȯ.
	output.pos = mul(output.pos, projection);		// ���� ��ȯ.

	// �������� �Է¹��� �ؽ�ó ��ǥ ��ȯ.
	output.texCoord = input.texCoord;

	return output;
}