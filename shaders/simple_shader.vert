#version 450
layout(location=0)in vec3 position;
layout(location=1)in vec3 color;
layout(location=2)in vec3 normal;
layout(location=3)in vec2 uv;

layout(location=0)out vec3 fragColor;
layout(location=1)out vec3 fragPosWorld;
layout(location=2)out vec3 fragNormalWorld;

layout(set=0,binding=0)uniform GlobalUbo{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 ambientLightColor;//w是intensity
    vec4 lightPosition;
    vec4 lightColor;
}ubo;

layout(push_constant)uniform Push{
	mat4 modelMatrix;//
	mat4 normalMatrix;//用于计算法向量在世界坐标下的坐标
}push;

void main() 
{
	vec3 posWorld=(push.modelMatrix*vec4(position,1.0f)).xyz;
	vec3 lightDirection=ubo.lightPosition.xyz-posWorld;
	gl_Position = vec4(ubo.projectionMatrix*ubo.viewMatrix*push.modelMatrix*vec4(position,1.0f));
	vec3 normalWorldSpace=normalize(push.normalMatrix*vec4(normal,0.0)).xyz;
	fragPosWorld=posWorld;
	fragNormalWorld=normalWorldSpace;
	fragColor=color;
	//fragColor=(ambientLight+diffuseLight*attenuation)*color;
}