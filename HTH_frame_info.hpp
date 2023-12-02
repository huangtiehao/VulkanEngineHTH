#pragma once
#include "HTH_camera.hpp"
#include<vulkan/vulkan.h>
#include"HTH_game_object.hpp"
#define MAX_LIGHTS 10
struct PointLight {
	glm::vec4 position{};
	glm::vec4 color{};
};
struct GlobalUbo {
	glm::mat4 projection{ 1.f };
	glm::mat4 view{ 1.f };
	glm::mat4 inverseView{ 1.f };
	glm::vec4 ambientLightColor{ 1.0f,1.0f,1.0f,0.02f };
	PointLight pointLights[MAX_LIGHTS];
	int numLights;
};
struct HTH_frame_info
{

	int frameIndex;
	float frameTime;
	VkCommandBuffer commandBuffer;
	HTH_camera& camera;
	VkDescriptorSet globalDescriptorSet;
	HTH_game_object::Map& gameObjects;
};