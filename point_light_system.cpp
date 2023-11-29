#include "point_light_system.hpp"
#include<stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<array>

Point_light_system::Point_light_system(HTH_device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) :hth_device{ device }
{
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}
Point_light_system::~Point_light_system()
{
	vkDestroyPipelineLayout(hth_device.device(), pipelineLayout, nullptr);
}


void Point_light_system::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
{
	//VkPushConstantRange pushConstantRange{};
	//pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	//pushConstantRange.offset = 0;
	//pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout>descriptorSetLayouts{ globalSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(hth_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}
void Point_light_system::render(HTH_frame_info& frameInfo)
{
	hth_pipeline->bind(frameInfo.commandBuffer);
	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1
		, &frameInfo.globalDescriptorSet, 0, nullptr);
	vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}
void Point_light_system::createPipeline(VkRenderPass renderPass)
{
	PipelineConfigInfo pipelineConfig{};
	HTH_pipeline::defaultPipelineConfigInfo(
		pipelineConfig);
	pipelineConfig.bindingDescriptions.clear();
	pipelineConfig.attributeDescriptions.clear();
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	hth_pipeline = std::make_unique<HTH_pipeline>(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/shaders/point_light.vert.spv",
		"C:/visual_studio_source_code/VulkanEngineHTH/shaders/point_light.frag.spv", pipelineConfig);
}
