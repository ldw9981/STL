
attribute vec3 inPosition;
attribute vec4 inColor0;
attribute vec2 inTexCoord;

varying float Fog;	
varying float4 Color;
varying float4 Tex0;
varying float4 SunLight;
varying float4 ShadowTexCoord;
#ifdef USE_ShadowTexelPos_INTERPOLATOR
varying float2 ShadowTexelPos;
#endif
varying float2 WorldPos;

void main()
{
	VS_OUTPUT_FLORA_MAP VSOut;
	VSOut = vs_flora_map(CURRENT_PCF_MODE, vec4(inPosition, 1.0), inColor0, inTexCoord);

	gl_Position = VSOut.Pos;
	Fog = VSOut.Fog;
	Color = VSOut.Color;
	Tex0 = VSOut.Tex0;
	SunLight = VSOut.SunLight;
	ShadowTexCoord = VSOut.ShadowTexCoord;
	#ifdef USE_ShadowTexelPos_INTERPOLATOR
		ShadowTexelPos = VSOut.ShadowTexelPos;
	#endif
	WorldPos = VSOut.WorldPos;
}
