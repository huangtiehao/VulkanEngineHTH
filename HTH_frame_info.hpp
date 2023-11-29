#pragma once
#include "HTH_camera.hpp"
#include<vulkan/vulkan.h>
#include"HTH_game_object.hpp"
struct HTH_frame_info
{
	int frameIndex;
	float frameTime;
	VkCommandBuffer commandBuffer;
	HTH_camera& camera;
	VkDescriptorSet globalDescriptorSet;
	HTH_game_object::Map& gameObjects;
};