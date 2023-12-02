#include "point_light_system.hpp"
#include<stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<array>

struct PointLightPushConstants {
	glm::vec4 position{};
	glm::vec4 color{};
	float radius;
};

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
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PointLightPushConstants);

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
void Point_light_system::render(HTH_frame_info& frameInfo)
{
	hth_pipeline->bind(frameInfo.commandBuffer);
	vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1
		, &frameInfo.globalDescriptorSet, 0, nullptr);

	for (auto& kv : frameInfo.gameObjects)
	{
		auto& obj = kv.second;
		if (obj.pointLight == nullptr)continue;
		PointLightPushConstants push{};
		push.position = glm::vec4(obj.transform.translation, 1.f);
		push.color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
		push.radius = obj.transform.scale.x;
		vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT
			, 0, sizeof(PointLightPushConstants), &push);
		vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
	}

}
void Point_light_system::createPipeline(VkRenderPass renderPass)
{
	PipelineConfigInfo pipelineConfig{};
	HTH_pipeline::defaultPipelineConfigInfo(
		pipelineConfig);
	HTH_pipeline::enableAlphaBlending(pipelineConfig);
	pipelineConfig.bindingDescriptions.clear();
	pipelineConfig.attributeDescriptions.clear();
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = pipelineLayout;
	hth_pipeline = std::make_unique<HTH_pipeline>(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/shaders/point_light.vert.spv",
		"C:/visual_studio_source_code/VulkanEngineHTH/shaders/point_light.frag.spv", pipelineConfig);
}
void Point_light_system::updateFrameInfo(HTH_frame_info& frameInfo, GlobalUbo& ubo)
{
	int lightIndex = 0;
	auto rotateLight = glm::rotate(glm::mat4{ 1.f }, frameInfo.frameTime, { 0.f,-1.f,-0.f });
	for (auto& kv : frameInfo.gameObjects)
	{
		auto& obj = kv.second;
		if (obj.pointLight == nullptr)continue;
		obj.transform.translation = glm::vec3{ rotateLight * glm::vec4(obj.transform.translation,1.f) };
		ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
		ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLight->lightIntensity);
		lightIndex += 1;
	}
	ubo.numLights=lightIndex;
}