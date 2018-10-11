// �������.
cbuffer cbPerObject
{
	matrix world;
	matrix view;
	matrix projection;
};

// ���� ���̴� �Է¿� ����ü.
struct vs_input
{
	float4 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

// ���� ���̴� ��¿� ����ü.
struct vs_output
{
	float4 pos : SV_POSITION;
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

// ���� ���̴�.
vs_output main(vs_input input)
{
	vs_output output;
	// ���� ��ȯ.
	output.pos = mul(input.pos, world);		// ���� ��ȯ.
	output.pos = mul(output.pos, view);		// �� ��ȯ.
	output.pos = mul(output.pos, projection);		// ���� ��ȯ.

	//output.color = color;

	// �������� �Է¹��� �ؽ�ó ��ǥ ��ȯ.
	output.texCoord = input.texCoord;

	// ��� ���� ��ȯ.
	output.normal = input.normal;

	return output;
}