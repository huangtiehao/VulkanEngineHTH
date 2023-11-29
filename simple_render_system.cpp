#include "simple_render_system.hpp"
#include<stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<array>

struct SimplePushConstantData {
	//µ•Œªæÿ’Û
	glm::mat4 modelMatrix{1.f};
	glm::mat4 normalMatrix{ 1.f };
};

Simple_render_system::Simple_render_system(HTH_device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout):hth_device{device}
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}
Simple_render_system::~Simple_render_system()
{
	vkDestroyPipelineLayout(hth_device.device(), pipelineLayout, nullptr);
}


void Simple_render_system::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout>descriptorSetLayouts{ globalSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(hth_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}
void Simple_render_system::renderGameObjects(HTH_frame_info& frameInfo)
{
	hth_pipeline->bind(frameInfo.commandBuffer);
	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1
		, &frameInfo.globalDescriptorSet, 0, nullptr);
	for (auto&kv :frameInfo.gameObjects )
	{
		auto& obj = kv.second;
		if (obj.model == nullptr)continue;
		SimplePushConstantData push{};
		push.normalMatrix = obj.transform.normalMatrix();
		push.modelMatrix = obj.transform.modelMatrix();
		vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
		obj.model->bind(frameInfo.commandBuffer);
		obj.model->draw(frameInfo.commandBuffer);
	}
}
void Simple_render_system::createPipeline(VkRenderPass renderPass)
{
	PipelineConfigInfo pipelineConfig{};
	HTH_pipeline::defaultPipelineConfigInfo(
		pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	hth_pipeline = std::make_unique<HTH_pipeline>(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/shaders/simple_shader.vert.spv",
		"C:/visual_studio_source_code/VulkanEngineHTH/shaders/simple_shader.frag.spv", pipelineConfig);
}
