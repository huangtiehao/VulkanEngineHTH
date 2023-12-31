#pragma once
#include "HTH_device.hpp"
#include "HTH_camera.hpp"
#include "HTH_pipeline.hpp"
#include "HTH_model.hpp"
#include "HTH_game_object.hpp"
#include "HTH_frame_info.hpp"

#include<memory>
#include<vector>
class Simple_render_system {
public:
	Simple_render_system(HTH_device& device,VkRenderPass renderPass,VkDescriptorSetLayout globalSetLayout);
	~Simple_render_system();
	Simple_render_system(const Simple_render_system&) = delete;
	Simple_render_system& operator=(const Simple_render_system&) = delete;
	void renderGameObjects(HTH_frame_info& frameInfo);
private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass);

	HTH_device& hth_device;
	std::unique_ptr<HTH_pipeline>hth_pipeline;
	VkPipelineLayout pipelineLayout;
};