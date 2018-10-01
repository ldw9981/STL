// 픽셀 셰이더 입력용 구조체.
struct ps_input
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal:NORMAL;
	float4 diffuse:TEXCOORD1;
};

// 텍스처 정보.
Texture2D objTexture;
SamplerState objSampler;

// 픽셀 셰이더(쉐이더/셰이더).
float4 main(ps_input input) : SV_TARGET
{
	// 텍스처 샘플링 (색상 정보 추출).
	
	float4 texColor = objTexture.Sample(
		objSampler, input.texCoord);
/*
	// 텍스처 색상 반환.
	return texColor;
	*/

	// 0~1 클램프
	float3 diffuse = saturate(input.diffuse);
	float3 finalColor = texColor.rgb * diffuse;

	return float4(finalColor, 1);
}