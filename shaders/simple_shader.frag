#version 450
layout(location=0)in vec3 fragColor;
layout(location=1)in vec3 fragPosWorld;
layout(location=2)in vec3 fragNormalWorld;

layout(location=0)out vec4 outColor;

layout(set=0,binding=0)uniform GlobalUbo{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 ambientLightColor;//w是intensity
    vec4 lightPosition;
    vec4 lightColor;
}ubo;

layout(push_constant)uniform Push{
	mat4 modelMatrix;
	mat4 normalMatrix;//用于计算法向量在世界坐标下的坐标
}push;
void main()
{
	vec3 lightDirection=ubo.lightPosition.xyz-fragPosWorld;
	float attenuation=1.0/dot(lightDirection,lightDirection);
	lightDirection=normalize(lightDirection);
	vec3 ambientColor=ubo.ambientLightColor.xyz*ubo.ambientLightColor.w;
	vec3 diffuseColor=max(dot(lightDirection,normalize(fragNormalWorld)),0)*ubo.lightColor.xyz*ubo.lightColor.w;
	outColor=vec4((ambientColor+diffuseColor*attenuation)*fragColor,1.0);
}