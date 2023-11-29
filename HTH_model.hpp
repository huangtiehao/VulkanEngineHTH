#pragma once
#include "HTH_device.hpp"
#include "HTH_buffer.hpp" 
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <vector>
#include <glm/glm.hpp>
#include<memory>


class HTH_model
{

public:
	struct Vertex {
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};
		bool operator==(const Vertex& other)const
		{
			return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
		}
	};
public:
	struct Builder {
		std::vector<Vertex>vertices{};
		std::vector<uint32_t>indices{};
		void loadModel(const std::string& filePath);
	};

public:

	HTH_model(HTH_device& device, const HTH_model::Builder& builder);
	~HTH_model();
	
	HTH_model(const HTH_model&) = delete;
	HTH_model &operator=(const HTH_model&) = delete;

	static std::unique_ptr<HTH_model>createModelFromFile(HTH_device& device, const std::string& filePath);

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);
private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<uint32_t>& indices);
	HTH_device& hth_device;

	std::unique_ptr<HTH_buffer> vertexBuffer;
	uint32_t vertexCount;

	bool hasIndexBuffer = false;
	std::unique_ptr<HTH_buffer> indexBuffer;
	uint32_t indexCount;
};