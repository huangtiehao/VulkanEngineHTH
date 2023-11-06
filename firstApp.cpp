#include "firstApp.hpp"
#include<stdexcept>
#include<array>
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
		drawFrame();
	}
	vkDeviceWaitIdle(hth_device.device());
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
	PipelineConfigInfo pipelineConfig{};
	HTH_pipeline::defaultPipelineConfigInfo(
		pipelineConfig,
		hth_swap_chain.width(),
		hth_swap_chain.height());
	pipelineConfig.renderPass = hth_swap_chain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	hth_pipeline = std::make_unique<HTH_pipeline>(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/shaders/vert.spv",
		"C:/visual_studio_source_code/VulkanEngineHTH/shaders/frag.spv", pipelineConfig);
}

void firstApp::createCommandBuffers()
{
	commandBuffers.resize(hth_swap_chain.imageCount());	
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;	
	allocInfo.commandPool = hth_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(hth_device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
	for (int i = 0; i < commandBuffers.size(); ++i)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin command buffer!");
		}
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = hth_swap_chain.getRenderPass();
		renderPassInfo.framebuffer = hth_swap_chain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = hth_swap_chain.getSwapChainExtent();

		std::array<VkClearValue, 2>clearValues{};
		clearValues[0].color = { 0.1,0.1,0.1,1.0 };
		clearValues[1].depthStencil = { 1.0f,0 };
		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		hth_pipeline->bind(commandBuffers[i]);
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to end command buffer!");
		}
	}
}

void firstApp::drawFrame()
{
	uint32_t imageIndex;
	auto result = hth_swap_chain.acquireNextImage(&imageIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire next image!");
	}
	result = hth_swap_chain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit command buffers!");
	}
}
