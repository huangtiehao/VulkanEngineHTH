#include "simple_render_system.hpp"
#include<stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<array>

struct SimplePushConstantData {
	//��λ����
	glm::mat2 transform{ 0.f,-1.f,1.f,0.f };
	glm::vec2 offset;
	alignas(16)glm::vec3 color;
};

Simple_render_system::Simple_render_system(HTH_device& device, VkRenderPass renderPass):hth_device{device}
{
	createPipelineLayout();
	createPipeline(renderPass);
}
Simple_render_system::~Simple_render_system()
{
	vkDestroyPipelineLayout(hth_device.device(), pipelineLayout, nullptr);
}


void Simple_render_system::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(hth_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}
void Simple_render_system::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<HTH_game_object>& gameObjects)
{
	hth_pipeline->bind(commandBuffer);
	for (auto& obj : gameObjects)
	{
		obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0001f, glm::two_pi<float>());
		SimplePushConstantData push{};
		push.offset = obj.transform2d.translation;
		push.color = obj.color;
		push.transform = obj.transform2d.mat2();
		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}
void Simple_render_system::createPipeline(VkRenderPass renderPass)
{
	PipelineConfigInfo pipelineConfig{};
	HTH_pipeline::defaultPipelineConfigInfo(
		pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	hth_pipeline = std::make_unique<HTH_pipeline>(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/shaders/vert.spv",
		"C:/visual_studio_source_code/VulkanEngineHTH/shaders/frag.spv", pipelineConfig);
}