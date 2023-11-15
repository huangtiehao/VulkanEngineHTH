#pragma once
#include "HTH_device.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <vector>
#include <glm/glm.hpp>


class HTH_model
{

public:
	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;
	};


	HTH_model(HTH_device& device, const std::vector<Vertex>vertices);
	~HTH_model();

	std::vector<VkVertexInputBindingDescription>bindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>attributeDescriptions;
	

	HTH_model(const HTH_model&) = delete;
	HTH_model &operator=(const HTH_model&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);
private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void initialDescriptions();
	HTH_device& hth_device;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;
};