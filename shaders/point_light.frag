//#version 450
//
//layout(location=0)in vec2 fragOffsets;
//layout(location=0)out vec4 outColor;
//
//layout(set=0,binding=0)uniform GlobalUbo{
//	mat4 projectionMatrix;
//	mat4 viewMatrix;
//	vec4 ambientLightColor;//wÊÇintensity
//    vec4 lightPosition;
//    vec4 lightColor;
//}ubo;
//
//const float LIGHT_RADIUS=1.0f;
//
//void main()
//{
//	if(sqrt(dot(fragOffsets,fragOffsets))>LIGHT_RADIUS)
//	{
//		discard;
//	}
//	outColor=vec4(1.0f);
//
//}
#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

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
    vec4 position;
    vec4 color;
    float radius;
}push;
const float pi=3.14;
void main() {
  float dis = sqrt(dot(fragOffset, fragOffset));
  if (dis >= 1.0) {
    discard;
  }
  outColor = vec4(push.color.xyz, 0.5*cos(dis*pi/2));
}