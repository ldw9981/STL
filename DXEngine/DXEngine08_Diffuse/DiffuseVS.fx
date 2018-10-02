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
	float3 diffuse : TEXCOORD1;
};

// ���� ���̴�.
vs_output main(vs_input input)
{
	vs_output output;
	// ���� ��ȯ.
	output.pos = mul(input.pos, world);		// ���� ��ȯ.

	// ����Ʈ ���� ���ϱ�.
	float4 lightDir = output.pos - worldLightPosition;
	
	// �������ͷ� ����� (����ȭ).
	lightDir = normalize(lightDir);

	// ���� ��ǥ�� ���� ��� �� ���ϱ�.
	float3 worldNormal = normalize(mul(input.normal, (float3x3)world));

	// ����.
	float3 diffuse = dot(-lightDir.xyz, worldNormal);
	/*diffuse = -lightDir.x * worldNormal.x + -lightDir.y * worldNormal.y + -lightDir.z * worldNormal.z;*/

	// ���ݻ籤 ��ȯ.
	/*output.diffuse = diffuse;*/
	output.diffuse = diffuse * 0.5f + 0.5f;

	output.pos = mul(output.pos, view);		// �� ��ȯ.
	output.pos = mul(output.pos, projection);		// ���� ��ȯ.

	//output.color = color;

	// �������� �Է¹��� �ؽ�ó ��ǥ ��ȯ.
	output.texCoord = input.texCoord;

	// ��� ���� ��ȯ.
	output.normal = input.normal;

	return output;
}