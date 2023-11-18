#pragma once
#include "HTH_device.hpp"
#include "HTH_window.hpp"
#include "HTH_swap_chain.hpp"
#include "HTH_model.hpp"
#include<memory>
#include<vector>
#include<cassert>
class HTH_renderer {
public:
	HTH_renderer(HTH_window& window,HTH_device& device);
	~HTH_renderer();
	HTH_renderer(const HTH_renderer&) = delete;
	HTH_renderer& operator=(const HTH_renderer&) = delete;

	VkRenderPass getSwapChainRenderPass()const { return hth_swap_chain->getRenderPass(); }
	float getAspectRatio()const { return hth_swap_chain->extentAspectRatio(); }
	bool isFrameInProgress()const { return isFrameStarted; }

	VkCommandBuffer getCurrentCommandBuffer()const { 
		assert(isFrameStarted&&"cannot get commandbuffer when not in progress");
		return commandBuffers[currentFrameIndex]; }
	
	int getFrameIndex()const;
	VkCommandBuffer beginFrame();
	void endFrame();

	void beginSwapchainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapchainRenderPass(VkCommandBuffer commandBuffer);
private:
	void createCommandBuffers();
	void recreateSwapchain();
	void freeCommandBuffers();

	bool isFrameStarted=false;
	uint32_t currentImageIndex;
	int currentFrameIndex=0;
	HTH_window& hth_window;
	HTH_device& hth_device;
	std::unique_ptr<HTH_swap_chain>hth_swap_chain;
	std::vector<VkCommandBuffer>commandBuffers;
};