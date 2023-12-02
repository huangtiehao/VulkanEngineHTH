#version 450
layout(location=0)in vec3 fragColor;
layout(location=1)in vec3 fragPosWorld;
layout(location=2)in vec3 fragNormalWorld;

layout(location=0)out vec4 outColor;

struct PointLight{
    vec4 position;
    vec4 color;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 inverseView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;

layout(push_constant)uniform Push{
	mat4 modelMatrix;
	mat4 normalMatrix;//用于计算法向量在世界坐标下的坐标
}push;
void main()
{
	vec3 diffuseColor=ubo.ambientLightColor.xyz*ubo.ambientLightColor.w;diffuseColor;
	vec3 eyePosWorld=(ubo.inverseView*vec4(0.f,0.f,0.f,1.f)).xyz;
	vec3 viewDirection=normalize(eyePosWorld-fragPosWorld);
	vec3 specularColor=vec3(0.f);
	for(int i=0;i<ubo.numLights;i++)
	{
		PointLight light=ubo.pointLights[i];
		vec3 lightDirection=light.position.xyz-fragPosWorld;
		float attenuation=1.0/dot(lightDirection,lightDirection);
		lightDirection=normalize(lightDirection);
		vec3 h=normalize(lightDirection+viewDirection);
		diffuseColor+=max(dot(lightDirection,normalize(fragNormalWorld)),0)*light.color.xyz*light.color.w*attenuation;
		specularColor+=pow(max(dot(h,normalize(fragNormalWorld)),0),32)*light.color.xyz*light.color.w*attenuation;
	}
	outColor=vec4(diffuseColor*fragColor+specularColor*fragColor,1.0);


}