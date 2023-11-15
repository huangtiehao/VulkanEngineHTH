#pragma once
#include "HTH_device.hpp"
#include "HTH_pipeline.hpp"
#include "HTH_model.hpp"
#include "HTH_game_object.hpp"

#include<memory>
#include<vector>
class Simple_render_system {
public:
	Simple_render_system(HTH_device& device,VkRenderPass renderPass);
	~Simple_render_system();
	Simple_render_system(const Simple_render_system&) = delete;
	Simple_render_system& operator=(const Simple_render_system&) = delete;
	void renderGameObjects(VkCommandBuffer commandBuffer,std::vector<HTH_game_object>& gameObjects);
private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);


	HTH_device& hth_device;
	std::unique_ptr<HTH_pipeline>hth_pipeline;
	VkPipelineLayout pipelineLayout;
};