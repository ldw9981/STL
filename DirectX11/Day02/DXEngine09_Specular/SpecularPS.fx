// 픽셀 셰이더 입력용 구조체.
struct ps_input
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal:NORMAL;
	float4 diffuse:TEXCOORD1;
	float4 viewDir : TEXCOORD2;
	float4 reflection : TEXCOORD3;
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
	
	// 스펙큘러
	float3 reflection = normalize(input.reflection).xyz;
	float3 viewDir = normalize(input.viewDir).xyz;
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		// 내적 뷰벡터,반사벡터
		specular = dot(reflection, -viewDir);
		specular = saturate(specular);
		specular = pow(specular, 20.0f); // 스펙큘러 파워 상수로 뺄수도 있다.
	}

	float3 finalColor = texColor.rgb * diffuse + specular;

	return float4(finalColor, 1.0f);
}