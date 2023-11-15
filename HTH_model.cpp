#include "HTH_model.hpp"
#include <cassert>
HTH_model::HTH_model(HTH_device& device,const std::vector<Vertex>vertices) :hth_device{ device }
{
	initialDescriptions();
	createVertexBuffers(vertices);
}
HTH_model::~HTH_model()
{
	vkDestroyBuffer(hth_device.device(), vertexBuffer, nullptr);
	vkFreeMemory(hth_device.device(), vertexBufferMemory, nullptr);
}
void HTH_model::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	assert(vertexCount >= 3&&"Vertex count must be at least 3");
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
	hth_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);
	void* data;
	vkMapMemory(hth_device.device(), vertexBufferMemory, 0, bufferSize,0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(hth_device.device(), vertexBufferMemory);
}

void HTH_model::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer,0,1,buffers,offsets);
}
void HTH_model::draw(VkCommandBuffer commandBuffer)
{
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}
void HTH_model::initialDescriptions()
{
	bindingDescriptions.resize(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(HTH_model::Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	attributeDescriptions.resize(2);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = 0;
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex,color);
}