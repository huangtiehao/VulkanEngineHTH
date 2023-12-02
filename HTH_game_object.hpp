#pragma once
#include "HTH_model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include<memory>
#include<unordered_map>
struct TransformComponent {
	glm::vec3 translation{};//position offset
	glm::vec3 scale{ 1.0f,1.0f,1.0f };
	glm::vec3 rotation{};
	glm::mat4 modelMatrix();
	glm::mat3 normalMatrix();
};
struct PointLightComponent {
	float lightIntensity = 1.0f;
};
class HTH_game_object {
public:

	using id_t = unsigned int;
	using Map = std::unordered_map<id_t, HTH_game_object>;
	static HTH_game_object createObject()
	{
		static id_t current_id = 0;
		return HTH_game_object{ current_id++ };
	}
	id_t getId()
	{
		return id;
	}

	static HTH_game_object makePointLight(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

	HTH_game_object(const HTH_game_object&) = delete;
	HTH_game_object& operator=(const HTH_game_object&) = delete;
	HTH_game_object(HTH_game_object&&) = default;
	HTH_game_object& operator=(HTH_game_object&&) = default;

	std::shared_ptr<HTH_model>model{};
	std::unique_ptr<PointLightComponent>pointLight = nullptr;
	glm::vec3 color{};
	TransformComponent transform{};
private:
	id_t id;
	HTH_game_object(id_t objId) :id{ objId }
	{

	}

};