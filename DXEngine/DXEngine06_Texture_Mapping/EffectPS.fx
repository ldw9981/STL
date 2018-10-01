// �ȼ� ���̴� �Է¿� ����ü.
struct ps_input
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD0;
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

	// �ؽ�ó ���� ��ȯ.
	return texColor;
}