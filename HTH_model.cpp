#include "HTH_model.hpp"
#include <cassert>
#include <iostream>
#include "HTH_utils.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include<tiny_obj_loader.h>
#include<unordered_map>
namespace std {
	template<>
	struct hash<HTH_model::Vertex>
	{
		size_t operator()(HTH_model::Vertex const& vertex)const {
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}
HTH_model::HTH_model(HTH_device& device, const HTH_model::Builder& builder) :hth_device{ device }
{
	createVertexBuffers(builder.vertices);
	createIndexBuffers(builder.indices);
}
HTH_model::~HTH_model()
{

}


void HTH_model::createVertexBuffers(const std::vector<Vertex>& vertices)
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	assert(vertexCount >= 3&&"Vertex count must be at least 3");
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

	uint32_t vertexSize = sizeof(vertices[0]);
	HTH_buffer stagingBuffer{ hth_device,vertexSize,vertexCount,VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 1};
	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)vertices.data());

	vertexBuffer = std::make_unique<HTH_buffer>( hth_device,vertexSize,vertexCount,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,1 );

	hth_device.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
	/*VkBuffer stagingBuffer;//暂存的gpu内存，还要传送拷贝到性能更高的vertexBuffer块
	VkDeviceMemory stagingBufferMemory;
	//VK_MEMORY_PROPERTY_HOST_COHERENT_BIT同步cpu与gpu的数据
	hth_device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
	void* data;
	vkMapMemory(hth_device.device(),stagingBufferMemory, 0, bufferSize,0, &data);//gpu的一块区域map到cpu
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));//往cpu内存里拷贝数据，会自动flush到gpu
	vkUnmapMemory(hth_device.device(), stagingBufferMemory);//cpu内存数据不再使用，于是便unmap

	hth_device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |VK_BUFFER_USAGE_TRANSFER_DST_BIT,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer, vertexBufferMemory);
	*/
}
void HTH_model::Builder::loadModel(const std::string& filePath)
{
	std::unordered_map<HTH_model::Vertex, uint32_t>uniqueVertices;
	tinyobj::attrib_t attrib;//color,normal,position.....
	std::vector<tinyobj::shape_t>shapes;//index information
	std::vector<tinyobj::material_t>materials;
	std::string warn, err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
	{
		throw std::runtime_error(warn+err);
	}
	vertices.clear();
	indices.clear();
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};
			if (index.vertex_index >= 0)
			{
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2] };

				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2] };

			}
			if (index.normal_index >= 0)
			{
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]};
			}
			if (index.texcoord_index >= 0)
			{
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]};
			}
			if (uniqueVertices.count(vertex)==0)
			{
				uniqueVertices[vertex] = uniqueVertices.size();
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

std::unique_ptr<HTH_model> HTH_model::createModelFromFile(HTH_device& device, const std::string& filePath)
{
	Builder builder{};
	builder.loadModel(filePath);
	std::cout << "Vertex Count: " << builder.vertices.size() << std::endl;
	std::cout << "Indices Count: " << builder.indices.size() << std::endl;
	return std::make_unique<HTH_model>(device, builder);
}

void HTH_model::createIndexBuffers(const std::vector<uint32_t>& indices)
{
	indexCount = static_cast<uint32_t>(indices.size());
	hasIndexBuffer = indexCount > 0;
	if (!hasIndexBuffer)return;
	VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
	uint32_t indexSize = sizeof(indices[0]);
	HTH_buffer stagingBuffer{ hth_device,indexSize,indexCount,VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ,1 };
	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)indices.data());

	indexBuffer=std::make_unique<HTH_buffer>(hth_device,indexSize,indexCount,VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,1 );
	/*VkBuffer stagingBuffer;//暂存的gpu内存，还要传送拷贝到性能更高的vertexBuffer块
	VkDeviceMemory stagingBufferMemory;
	//VK_MEMORY_PROPERTY_HOST_COHERENT_BIT同步cpu与gpu的数据
	hth_device.createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
	void* data;
	vkMapMemory(hth_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);//gpu的一块区域map到cpu
	memcpy(data, indices.data(), static_cast<size_t>(bufferSize));//往cpu内存里拷贝数据，会自动flush到gpu
	vkUnmapMemory(hth_device.device(),stagingBufferMemory);//cpu内存数据不再使用，于是便unmap

	hth_device.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT|VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBuffer, indexBufferMemory);
	*/
	hth_device.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
}

void HTH_model::bind(VkCommandBuffer commandBuffer)
{
	VkBuffer buffers[] = { vertexBuffer->getBuffer()};
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer,0,1,buffers,offsets);
	if (hasIndexBuffer)
	{
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}
}
void HTH_model::draw(VkCommandBuffer commandBuffer)
{
	if (hasIndexBuffer)
	{
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}
	else
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

}
