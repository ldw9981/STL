// �������.
cbuffer cbPerObject
{
	matrix world;
	matrix view;
	matrix projection;
};

// ���� ���̴� ��¿� ����ü.
struct vs_output
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
};

// ���� ���̴�.
vs_output main(float4 pos : POSITION,
	float4 color : COLOR, float2 texCoord : TEXCOORD0)
{
	vs_output output;
	// ���� ��ȯ.
	output.pos = mul(pos, world);		// ���� ��ȯ.
	output.pos = mul(output.pos, view);		// �� ��ȯ.
	output.pos = mul(output.pos, projection);		// ���� ��ȯ.

	output.color = color;

	// �������� �Է¹��� �ؽ�ó ��ǥ ��ȯ.
	output.texCoord = texCoord;

	return output;
}