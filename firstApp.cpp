#include "firstApp.hpp"
#include<stdexcept>

firstApp::firstApp()
{
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}
firstApp::~firstApp()
{
	vkDestroyPipelineLayout(hth_device.device(), pipelineLayout, nullptr);
}
void firstApp::run()
{
	while (!hth_window.shouldClose())
	{
		glfwPollEvents();
	}
}

void firstApp::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(hth_device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void firstApp::createPipeline()
{
	PipelineConfigInfo pipelineConfig = HTH_pipeline::defaultPipelineConfigInfo(hth_swap_chain.width(), hth_swap_chain.height());
	pipelineConfig.renderPass = hth_swap_chain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	hth_pipeline = std::make_unique<HTH_pipeline>(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/shaders/vert.spv",
		"C:/visual_studio_source_code/VulkanEngineHTH/shaders/frag.spv", pipelineConfig);
}

void firstApp::createCommandBuffers()
{
}

void firstApp::drawFrame()
{
}
