// �ȼ� ���̴� �Է¿� ����ü.
struct ps_input
{
	float4 pos : SV_POSITION;
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;

	float3 diffuse : TEXCOORD1;
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

	float3 finalColor = diffuse;

	// �ؽ�ó ���� ��ȯ.
	//return texColor;
	return float4(finalColor, 1);
}