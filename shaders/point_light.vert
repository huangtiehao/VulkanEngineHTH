//#version 450
//const vec2 OFFSETS[6] = vec2[](
//  vec2(-1.0, -1.0),
//  vec2(-1.0, 1.0),
//  vec2(1.0, -1.0),
//  vec2(1.0, -1.0),
//  vec2(-1.0, 1.0),
//  vec2(1.0, 1.0)
//);
//layout(location=0)out vec2 fragOffsets;
//
//layout(set=0,binding=0)uniform GlobalUbo{
//	mat4 projectionMatrix;
//	mat4 viewMatrix;
//	vec4 ambientLightColor;//w «intensity
//    vec4 lightPosition;
//    vec4 lightColor;
//}ubo;	
//
//const float LIGHT_RADIUS=1.0;
//
//void main()
//{
//	fragOffsets=OFFSETS[gl_VertexIndex];
//	vec3 cameraRightWorld={ubo.viewMatrix[0][0],ubo.viewMatrix[1][0],ubo.viewMatrix[2][0]};
//	vec3 cameraUpWorld={ubo.viewMatrix[0][1],ubo.viewMatrix[1][1],ubo.viewMatrix[2][1]};
//	vec3 positionWorld=ubo.lightPosition.xyz+LIGHT_RADIUS*fragOffsets.x*cameraRightWorld+LIGHT_RADIUS*fragOffsets.y*cameraUpWorld;
//	gl_Position=ubo.projectionMatrix*ubo.viewMatrix*vec4(positionWorld,1.0);
//}
#version 450

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  vec4 ambientLightColor; // w is intensity
  vec3 lightPosition;
  vec4 lightColor;
} ubo;

const float LIGHT_RADIUS = 0.05;

void main() {
  fragOffset = OFFSETS[gl_VertexIndex];
  vec3 cameraRightWorld = {ubo.view[0][0], ubo.view[1][0], ubo.view[2][0]};
  vec3 cameraUpWorld = {ubo.view[0][1], ubo.view[1][1], ubo.view[2][1]};

  vec3 positionWorld = ubo.lightPosition.xyz
    + LIGHT_RADIUS * fragOffset.x * cameraRightWorld
    + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;

  gl_Position = ubo.projection * ubo.view * vec4(positionWorld, 1.0);
}