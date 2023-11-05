#pragma once
#include "HTH_device.hpp"
#include "HTH_window.hpp"
#include "HTH_pipeline.hpp"
#include "HTH_swap_chain.hpp"
#include<memory>
#include<vector>
class firstApp {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	firstApp();
	~firstApp();
	firstApp(const firstApp&) = delete;
	firstApp &operator=(const firstApp&) = delete;
	void run();
	
private:
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();

	HTH_window hth_window{WIDTH, HEIGHT, "Hello Vulkan!"};
	HTH_device hth_device{ hth_window };
	HTH_swap_chain hth_swap_chain{ hth_device,hth_window.getExtent() };
	std::unique_ptr<HTH_pipeline>hth_pipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer>commandBuffers;
};