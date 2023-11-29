#pragma once
#include<string>
#include<vector>
#include "HTH_device.hpp"
#include "HTH_model.hpp"

struct PipelineConfigInfo {
	PipelineConfigInfo(const PipelineConfigInfo&) = delete; 
	PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

	std::vector<VkVertexInputBindingDescription>bindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>attributeDescriptions;	
	VkPipelineViewportStateCreateInfo viewportInfo;
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo rasterizationInfo;
	VkPipelineMultisampleStateCreateInfo multisampleInfo;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo colorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
	std::vector<VkDynamicState>dynamicStateEnables;
	VkPipelineDynamicStateCreateInfo dynamicStateInfo;
	VkPipelineLayout pipelineLayout = nullptr;
	VkRenderPass renderPass = nullptr;
	uint32_t subpass = 0;
};

class HTH_pipeline {
public:
	HTH_pipeline(HTH_device& device,const std::string& vertFilePath, const std::string& fragFilePath,const PipelineConfigInfo& configInfo);
	~HTH_pipeline();
	HTH_pipeline(const HTH_pipeline&) = delete;

	HTH_pipeline& operator=(const HTH_pipeline&) = delete;
	void bind(VkCommandBuffer commandBuffer);
	static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
private:
	static std::vector<char> readFile(const std::string& filePath);
	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath,const PipelineConfigInfo& configInfo);
	HTH_device& hth_device;
	VkPipeline graphicsPipeline;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};