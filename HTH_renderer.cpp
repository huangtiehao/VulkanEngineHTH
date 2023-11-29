#include "HTH_renderer.hpp"
#include<stdexcept>
#include<array>

HTH_renderer::HTH_renderer(HTH_window& window, HTH_device& device):hth_window{window},hth_device(device)
{
	recreateSwapchain();
	createCommandBuffers();
}
HTH_renderer::~HTH_renderer()
{
	freeCommandBuffers();
}

void HTH_renderer::freeCommandBuffers() {
	vkFreeCommandBuffers(
		hth_device.device(),
		hth_device.getCommandPool(),
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.data());
	commandBuffers.clear();
}

int HTH_renderer::getFrameIndex() const
{
	assert(isFrameStarted && "cannot get frame index while not in progress");
	return currentFrameIndex;
}

VkCommandBuffer HTH_renderer::beginFrame()
{
	assert(!isFrameStarted && "can't begin frame while frame is started");
	auto result = hth_swap_chain->acquireNextImage(&currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapchain();
		return nullptr;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire next image!");
	}
	isFrameStarted = true;
	auto commandBuffer = getCurrentCommandBuffer();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin command buffer!");
	}
	return commandBuffer;
}

void HTH_renderer::endFrame()
{
	assert(isFrameStarted && "cannot end frame while is not started");
	auto commandBuffer = getCurrentCommandBuffer();
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to end command buffer!");
	}
	auto result = hth_swap_chain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || hth_window.wasWindowResized())
	{
		hth_window.resetWindowResizedFlag();
		recreateSwapchain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit command buffers!");
	}
	isFrameStarted = false;
	currentFrameIndex = (currentFrameIndex + 1) % HTH_swap_chain::MAX_FRAMES_IN_FLIGHT;
}

void HTH_renderer::beginSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "can't call beginSwapchainRenderpass if frame is not in progress");
	assert(commandBuffer==getCurrentCommandBuffer()&&"can't begin render pass in different frame");

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = hth_swap_chain->getRenderPass();
	renderPassInfo.framebuffer = hth_swap_chain->getFrameBuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = hth_swap_chain->getSwapChainExtent();

	std::array<VkClearValue, 2>clearValues{};
	clearValues[0].color = { 0.01f,0.01f,0.01f,1.0 };
	clearValues[1].depthStencil = { 1.0f,0 };
	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(hth_swap_chain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(hth_swap_chain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, hth_swap_chain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void HTH_renderer::endSwapchainRenderPass(VkCommandBuffer commandBuffer)
{
	assert(isFrameStarted && "can't call beginSwapchainRenderpass if frame is not in progress");
	assert(commandBuffer == getCurrentCommandBuffer() && "can't begin render pass in different frame");
	
	vkCmdEndRenderPass(commandBuffer);
}

void HTH_renderer::createCommandBuffers()
{
	commandBuffers.resize(HTH_swap_chain::MAX_FRAMES_IN_FLIGHT);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = hth_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(hth_device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffers!");
	}
}


void HTH_renderer::recreateSwapchain()
{
	auto extent = hth_window.getExtent();
	while (extent.height == 0 || extent.width == 0)
	{
		extent = hth_window.getExtent();
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(hth_device.device());
	if (hth_swap_chain == nullptr) {
		hth_swap_chain = std::make_unique<HTH_swap_chain>(hth_device, extent);
	}
	else 
	{
		std::shared_ptr<HTH_swap_chain>oldSwapChain = std::move(hth_swap_chain);
		hth_swap_chain = std::make_unique<HTH_swap_chain>(hth_device, extent, oldSwapChain);
		if (!oldSwapChain->compareSwapFormats(*hth_swap_chain.get()))
		{
			throw std::runtime_error("swapChain image format has changed");
		}
	}
}

