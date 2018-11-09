uniform vec4 vLightDiffuse[4];
uniform vec4 vMaterialColor;
uniform vec4 vSunDir;
uniform vec4 vSunColor;
uniform vec4 vAmbientColor;
uniform vec4 vSkyLightDir;
uniform vec4 vSkyLightColor;
uniform float fFogDensity;
uniform int iLightPointCount;
uniform int iLightIndices[4];
uniform mat4 matWorldViewProj;
uniform mat4 matWorldView;
uniform mat4 matWorld;
uniform vec4 vLightPosDir[4];
attribute vec3 inPosition;
attribute vec3 inNormal;
attribute vec2 inTexCoord;
attribute vec4 inColor0;
attribute vec4 inColor1;
varying vec4 Color;
varying vec4 Tex0;
varying vec4 SunLight;
varying vec4 ShadowTexCoord;
varying vec2 ShadowTexelPos;
varying vec4 WorldPos;
varying float Fog;
void main ()
{
  vec4 tmpvar_1;
  tmpvar_1.w = 1.0;
  tmpvar_1.xyz = inPosition;
  vec4 diffuse_light_2;
  vec4 tmpvar_3;
  vec4 tmpvar_4;
  vec4 tmpvar_5;
  vec2 tmpvar_6;
  vec4 tmpvar_7;
  tmpvar_3 = (matWorldViewProj * tmpvar_1);
  vec4 tmpvar_8;
  tmpvar_8 = (matWorld * tmpvar_1);
  vec4 tmpvar_9;
  tmpvar_9.w = 0.0;
  tmpvar_9.xyz = inNormal;
  vec3 tmpvar_10;
  tmpvar_10 = normalize((matWorld * tmpvar_9).xyz);
  tmpvar_4.xy = inTexCoord;
  tmpvar_4.z = (0.7 * (tmpvar_8.z - 1.5));
  tmpvar_4.w = tmpvar_8.x;
  diffuse_light_2 = (vAmbientColor + inColor1.zyxw);
  diffuse_light_2 = (diffuse_light_2 + (clamp (
    dot (tmpvar_10, -(vSkyLightDir.xyz))
  , 0.0, 1.0) * vSkyLightColor));
  vec4 vWorldPos_11;
  vWorldPos_11 = tmpvar_8;
  vec3 vWorldN_12;
  vWorldN_12 = tmpvar_10;
  vec4 total_14;
  total_14 = vec4(0.0, 0.0, 0.0, 0.0);
  for (int j_13 = 0; j_13 < iLightPointCount; j_13++) {
    int tmpvar_15;
    tmpvar_15 = iLightIndices[j_13];
    vec3 tmpvar_16;
    tmpvar_16 = (vLightPosDir[tmpvar_15].xyz - vWorldPos_11.xyz);
    total_14 = (total_14 + ((
      clamp (dot (vWorldN_12, normalize(tmpvar_16)), 0.0, 1.0)
     * vLightDiffuse[tmpvar_15]) * (1.0/(
      dot (tmpvar_16, tmpvar_16)
    ))));
  };
  diffuse_light_2 = (diffuse_light_2 + total_14);
  vec3 tmpvar_17;
  tmpvar_17 = (matWorldView * tmpvar_1).xyz;
  gl_Position = tmpvar_3;
  Color = ((vMaterialColor * inColor0) * diffuse_light_2);
  Tex0 = tmpvar_4;
  SunLight = (((
    clamp (dot (tmpvar_10, -(vSunDir.xyz)), 0.0, 1.0)
   * vSunColor) * vMaterialColor) * inColor0);
  ShadowTexCoord = tmpvar_5;
  ShadowTexelPos = tmpvar_6;
  WorldPos = tmpvar_7;
  Fog = (1.0/(exp2((
    sqrt(dot (tmpvar_17, tmpvar_17))
   * fFogDensity))));
}

