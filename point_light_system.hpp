#pragma once
#include "HTH_device.hpp"
#include "HTH_camera.hpp"
#include "HTH_pipeline.hpp"
#include "HTH_model.hpp"
#include "HTH_game_object.hpp"
#include "HTH_frame_info.hpp"
#include<memory>
#include<vector>
class Point_light_system {
public:
	Point_light_system(HTH_device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
	~Point_light_system();

	Point_light_system(const Point_light_system&) = delete;
	Point_light_system& operator=(const Point_light_system&) = delete;
	void updateFrameInfo(HTH_frame_info& frameInfo, GlobalUbo& ubo);
	void render(HTH_frame_info& frameInfo);
private:
	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass);

	HTH_device& hth_device;
	std::unique_ptr<HTH_pipeline>hth_pipeline;
	VkPipelineLayout pipelineLayout;
};