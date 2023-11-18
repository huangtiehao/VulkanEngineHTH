#include "HTH_camera.hpp"
#include<cassert>
#include<limits>
void HTH_camera::setOrthoGraphicProjection(float left, float right, float top, float bottom, float near, float far)
{
	glm::mat4 translate{ 1.0f };
	translate[3][0] = -(left + right) / 2.0;
	translate[3][1] = -(top + bottom) / 2.0;
	translate[3][2] = -(near + far) / 2.0;
	glm::mat4 scale{ 1.0f };
	scale[0][0] = 2.0 / (right-left);
	scale[1][1] = 2.0 / (bottom-top);
	scale[2][2] = 1.0 / (far - near);
	scale[3][2] = 0.5;
	projectionMatrix = scale * translate;
}

void HTH_camera::setPerspectiveProjection(float fovy, float aspectRatio, float near, float far)
{
	/*assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
	const float tanHalfFovy = tan(fovy / 2.f);
	projectionMatrix = glm::mat4{ 0.0f };
	projectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
	projectionMatrix[1][1] = 1.f / (tanHalfFovy);
	projectionMatrix[2][2] = far / (far - near);
	projectionMatrix[2][3] = 1.f;
	projectionMatrix[3][2] = -(far * near) / (far - near);*/
	glm::mat4 project{ 1.0f };
	project[0][0] = near;
	project[1][1] = near;
	project[2][2] = near + far;
	project[3][2] = -near * far;
	project[2][3] = 1;
	const float tanHalfFovy = tan(fovy / 2.f);
	float left = -near * aspectRatio * tanHalfFovy;
	float right = -left;
	float top= -near * tanHalfFovy;
	float bottom = -top;
	setOrthoGraphicProjection(left, right, top, bottom, near, far);
	projectionMatrix = projectionMatrix*project;
}

void HTH_camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	glm::mat4 translate{ 1.f };
	translate[3][0] = -position.x;
	translate[3][1] = -position.y;
	translate[3][2] = -position.z;
	glm::mat4 rotation{ 1.f };
	glm::vec3 z{ glm::normalize(direction) };
	glm::vec3 x{ glm::normalize(glm::cross(z,up)) };
	glm::vec3 y{ glm::normalize(glm::cross(z,x)) };
	rotation[0][0] = x.x;
	rotation[1][0] = x.y;
	rotation[2][0] = x.z;
	rotation[0][1] = y.x;
	rotation[1][1] = y.y;
	rotation[2][1] = y.z;
	rotation[0][2] = z.x;
	rotation[1][2] = z.y;
	rotation[2][2] = z.z;
	viewMatrix = rotation * translate;
}

void HTH_camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	setViewDirection(position, target - position, up);
}

void HTH_camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation)
{
	const float c3 = glm::cos(rotation.z);
	const float s3 = glm::sin(rotation.z);
	const float c2 = glm::cos(rotation.x);
	const float s2 = glm::sin(rotation.x);
	const float c1 = glm::cos(rotation.y);
	const float s1 = glm::sin(rotation.y);
	const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
	const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
	const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
	viewMatrix = glm::mat4{ 1.f };
	viewMatrix[0][0] = u.x;
	viewMatrix[1][0] = u.y;
	viewMatrix[2][0] = u.z;
	viewMatrix[0][1] = v.x;
	viewMatrix[1][1] = v.y;
	viewMatrix[2][1] = v.z;
	viewMatrix[0][2] = w.x;
	viewMatrix[1][2] = w.y;
	viewMatrix[2][2] = w.z;
	viewMatrix[3][0] = -glm::dot(u, position);
	viewMatrix[3][1] = -glm::dot(v, position);
	viewMatrix[3][2] = -glm::dot(w, position);
}
